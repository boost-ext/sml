//
// Copyright (c) 2022 Kris Jusiak (kris at jusiak dot net)
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//
// Usage
// #![feature(specialization)]
// #![allow(incomplete_features)]
//
// #[macro_use]
// mod sml;
// use sml::HList;
// use sml::ProcessEvent;
pub struct Nil;
pub struct Cons<H, T>(H, T);
pub trait HList: Sized { fn add<N>(self, item: N) -> Cons<N, Self> { Cons(item, self) } }
impl HList for Nil {}
impl<H, T> HList for Cons<H, T> {}

pub const fn hash(s: &str) -> u64 {
	let s = s.as_bytes();
	let mut i = 0usize;
	let mut hash = 0u32;
	while i < s.len() {
		hash = hash ^ (s[i] as u32) << 1;
		i += 1;
	}
	return hash as u64;
}

pub struct Transition<const SRC:u64, TEvent, const DST:u64> {
    pub guard: fn(&TEvent) -> bool,
    pub action: fn(&TEvent),
}

trait Dispatch {
    fn dispatch<TEvent>(&self, _event: &TEvent, _state: &mut u64);
}
impl Dispatch for Nil {
    fn dispatch<TEvent>(&self, _event: &TEvent, _state: &mut u64) {}
}
impl<T : DispatchImpl, Tail : Dispatch> Dispatch for Cons<T, Tail> {
    fn dispatch<TEvent>(&self, event:&TEvent, state: &mut u64) {
        self.1.dispatch(event, state);
        self.0.dispatch(event, state);
    }
}
impl<T : Sized, Tail : Dispatch> Dispatch for Cons<T, Tail> {
    default fn dispatch<TEvent>(&self, event:&TEvent, state: &mut u64) {
        self.1.dispatch(event, state);
    }
}

trait DispatchImpl {
    fn dispatch<TEvent>(&self, _event: &TEvent, _state: &mut u64);
}
impl<const SRC:u64, TEvent, const DST:u64> DispatchImpl for Transition<SRC, TEvent, DST> {
    fn dispatch<T>(&self, event: &T, state: &mut u64) {
        trait IsSame<U: ?Sized> { fn dispatch(f: impl FnMut()); }
        impl<T: ?Sized, U: ?Sized> IsSame<U> for T { default fn dispatch(_: impl FnMut()){} }
        impl<T: ?Sized> IsSame<T> for T { fn dispatch(mut f: impl FnMut() ){ f(); } }

        <T as IsSame<TEvent>>::dispatch(||{
            if SRC == *state && (self.guard)(unsafe { std::mem::transmute(event) }) {
                if DST != 0 {
                    *state = DST;
                }
                (self.action)(unsafe { std::mem::transmute(event) });
            }
        });
    }
}

pub struct StateMachine<T> {
    transition_table: T,
    state: u64,
}

pub trait InitialState {
    fn initial_state(&self) -> u64;
}
impl InitialState for Nil {
    fn initial_state(&self) -> u64 { 0 }
}
impl<T : InitialStateImpl, Tail : InitialState> InitialState for Cons<T, Tail> {
    fn initial_state(&self) -> u64 {
        self.0.initial_state() + self.1.initial_state()
    }
}

trait InitialStateImpl {
    fn initial_state(&self) -> u64;
}
impl InitialStateImpl for u64 {
    fn initial_state(&self) -> u64 { *self }
}
impl<const SRC:u64, TEvent, const DST:u64> InitialStateImpl for Transition<SRC, TEvent, DST> {
    fn initial_state(&self) -> u64 { 0 }
}

fn initial_state<TransitionTable : InitialState>(transition_table:&TransitionTable) -> u64 {
    transition_table.initial_state()
}

impl<T:InitialState> StateMachine<T> {
    pub fn new(transition_table:T) -> StateMachine<T> {
        let initial_state = initial_state(&transition_table);
        Self{transition_table:transition_table, state:initial_state}
    }
}

pub trait ProcessEvent {
    fn process_event<TEvent>(&mut self, _event: &TEvent);
}
impl<T : Dispatch> ProcessEvent for StateMachine<T> {
    fn process_event<TEvent>(&mut self, event: &TEvent) {
        self.transition_table.dispatch(event, &mut self.state);
    }
}

macro_rules! transition_table_impl {
    [$ts:expr,] => { $ts };

    [$ts:expr, $src:ident + $event:ident = $dst:ident, $($tail:tt)*] => {
        transition_table_impl![$ts.add(sml::Transition::<{sml::hash(stringify!($src))}, $event, {sml::hash(stringify!($dst))}>{guard:|_:&$event|{true}, action:|_:&$event|{}}), $($tail)*]
    };

    [$ts:expr, $src:ident + $event:ident / ($action:expr), $($tail:tt)*] => {
        transition_table_impl![$ts.add(sml::Transition::<{sml::hash(stringify!($src))}, $event, 0>{guard:|_:&$event|{true}|, action:$action}), $($tail)*]
    };

    [$ts:expr, $src:ident + $event:ident [ $guard:expr ] / ($action:expr), $($tail:tt)*] => {
        transition_table_impl![$ts.add(sml::Transition::<{sml::hash(stringify!($src))}, $event, 0>{guard:$guard, action:$action}), $($tail)*]
    };

    [$ts:expr, $src:ident + $event:ident / ($action:expr) = $dst:ident, $($tail:tt)*] => {
        transition_table_impl![$ts.add(sml::Transition::<{sml::hash(stringify!($src))}, $event, {sml::hash(stringify!($dst))}>{guard:|_:&$event|{true}, action:$action}), $($tail)*]
    };

    [$ts:expr, $src:ident + $event:ident [ $guard:expr ] = $dst:ident, $($tail:tt)*] => {
        transition_table_impl![$ts.add(sml::Transition::<{sml::hash(stringify!($src))}, $event, {sml::hash(stringify!($dst))}>{guard:$guard, action:|_:&$event|{}}), $($tail)*]
    };

    [$ts:expr, $src:ident + $event:ident [ $guard:expr ] / ($action:expr) = $dst:ident, $($tail:tt)*] => {
        transition_table_impl![$ts.add(sml::Transition::<{sml::hash(stringify!($src))}, $event, {sml::hash(stringify!($dst))}>{guard:$guard, action:$action}), $($tail)*]
    };

    [$ts:expr, * $src:ident $($tail:tt)*] => {
        transition_table_impl![$ts.add({sml::hash(stringify!($src))}), $src $($tail)*]
    };
}

macro_rules! transition_table {
     [$($body:tt)*] => {
         transition_table_impl!(sml::Nil, $($body)*)
     };
}
