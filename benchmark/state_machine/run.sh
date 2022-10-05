rm -f *.dat *.out out number
python3 gen.py

for ((i=1; i <= 500; i+=25)); do
  echo $i
  echo $i >> number
  #/usr/bin/time -f %e clang++-16 -I. -fbracket-depth=1024 -std=c++20 sml$i.cpp &>> sml.dat
  /usr/bin/time -f %e clang++-16 -I../../sml -fbracket-depth=1024 -std=c++20 sml.lite$i.cpp &>> sml.lite..dat
  /usr/bin/time -f %e rustc sml$i.rs &>> sml.rs.dat
done

echo -n "num " >> out
echo *.dat | sed "s/.dat//g"  >> out
paste -d ' ' number *.dat >> out
