var cpp_code = Array();
var cpp_output = Array();

function get_cpp_file(file) {
     var cpp_file = new XMLHttpRequest();
     cpp_file.open("GET", file, false);
     cpp_file.send();
     return cpp_file.responseText;
}

function toggle(id, file) {
    document.getElementById("run_it_btn_" + id).firstChild.data = 'Run this code!';
    document.getElementById("run_it_btn_" + id).removeChild;
    document.getElementById("run_it_btn_" + id).onclick = function() { show(id, file); };
    document.getElementById("code_listing_" + id).style.display = 'block';
    cpp_code[id].toTextArea();
    document.getElementById("code_" + id).style.display = 'none';
    cpp_output[id].toTextArea();
    document.getElementById("output_" + id).style.display = 'none';
    document.getElementById("compile_and_run_" + id).remove();
    document.getElementById("compiler_" + id).remove();
    document.getElementById("log_level_" + id).remove();
    document.getElementById("version_" + id).remove();
    document.getElementById("powered_by_" + id).remove();
}

function show(id, file) {
    console.log("a");
    document.getElementById('run_it_btn_' + id).firstChild.data = 'Exit';
    document.getElementById("run_it_btn_" + id).onclick = function() { toggle(id, file); };
    document.getElementById("code_listing_" + id).style.display = 'none';

    var powered_by = document.createElement("text");
    var powered_text = document.createTextNode("Powered by Wandbox.");
    powered_by.appendChild(powered_text);
    powered_by.setAttribute("id", "powered_by_" + id);
    powered_by.setAttribute('style', 'font-size: 10px; text-align: right; display: block;');

    var compile_btn = document.createElement("BUTTON");
    var compile_txt = document.createTextNode("Compile & Run (Ctrl+Enter)");
    compile_btn.setAttribute("id", "compile_and_run_" + id);
    compile_btn.setAttribute("class", "TryItBtn");
    compile_btn.appendChild(compile_txt);
    compile_btn.onclick = function() { compile_and_run(id) };

    var compiler_btn = document.createElement("SELECT");
    compiler_btn.setAttribute("id", "compiler_" + id);
    compiler_btn.setAttribute("class", "Option");
	{
	var option = document.createElement("OPTION");
	option.value = "clang-head";
	option.text = "clang-head";
	compiler_btn.appendChild(option);
	}
	{
	var option = document.createElement("OPTION");
	option.value = "clang-3.7";
	option.text = "clang-3.7";
	compiler_btn.appendChild(option);
	}
	{
	var option = document.createElement("OPTION");
	option.value = "clang-3.6";
	option.text = "clang-3.6";
	compiler_btn.appendChild(option);
	}
	{
	var option = document.createElement("OPTION");
	option.value = "clang-3.5";
	option.text = "clang-3.5";
	compiler_btn.appendChild(option);
	}
	{
	var option = document.createElement("OPTION");
	option.value = "clang-3.4";
	option.text = "clang-3.4";
	compiler_btn.appendChild(option);
	}
	{
	var option = document.createElement("OPTION");
	option.value = "gcc-head";
	option.text = "gcc-head";
	compiler_btn.appendChild(option);
	}
	{
	var option = document.createElement("OPTION");
	option.value = "gcc-5.2";
	option.text = "gcc-5.2";
	compiler_btn.appendChild(option);
	}
	{
	var option = document.createElement("OPTION");
	option.value = "gcc-5.1";
	option.text = "gcc-5.1";
	compiler_btn.appendChild(option);
	}

    compiler_btn.onclick = function() { };

    var log_level_btn = document.createElement("SELECT");

    log_level_btn.setAttribute("id", "log_level_" + id);
    log_level_btn.setAttribute("class", "Option");
	{
	var option = document.createElement("OPTION");
	option.text = "BOOST_DI_CFG_DIAGNOSTICS_LEVEL=0";
	option.value = "0";
	log_level_btn.appendChild(option);
	}
	{
	var option = document.createElement("OPTION");
	option.text = "BOOST_DI_CFG_DIAGNOSTICS_LEVEL=1 (default)";
	option.value = "1";
	log_level_btn.appendChild(option);
	}
	{
	var option = document.createElement("OPTION");
	option.text = "BOOST_DI_CFG_DIAGNOSTICS_LEVEL=2";
	option.value = "2";
	log_level_btn.appendChild(option);
	}

	log_level_btn.selectedIndex = 1;

    log_level_btn.onclick = function() { };

    var version_btn = document.createElement("SELECT");
    version_btn.setAttribute("id", "version_" + id);
    version_btn.setAttribute("class", "Option");
	{
	var option = document.createElement("OPTION");
	option.text = "head";
	option.value = "cpp14";
    version_btn.appendChild(option);
	}
	{
	var option = document.createElement("OPTION");
	option.text = "v0.5.0";
	option.value = "v0.5.0";
    version_btn.appendChild(option);
	}
    version_btn.onclick = function() { };

    document.getElementById("run_it_btn_" + id).parentNode.insertBefore(compile_btn, document.getElementById("run_it_btn_" + id).nextSibling);
    document.getElementById("run_it_btn_" + id).parentNode.insertBefore(compiler_btn, compile_btn.nextSibling);
    document.getElementById("run_it_btn_" + id).parentNode.insertBefore(log_level_btn, compiler_btn.nextSibling);
    document.getElementById("run_it_btn_" + id).parentNode.insertBefore(version_btn, log_level_btn.nextSibling);
    document.getElementById("run_it_btn_" + id).parentNode.insertBefore(powered_by, version_btn.nextSibling);

    get_example(id, file);
    compile_and_run(id);
}

function compile_and_run(id) {
    document.getElementById("compile_and_run_" + id).firstChild.data = "Compiling...";
    cpp_output[id].setValue("");
    var http = new XMLHttpRequest();
    http.open("POST", "http://melpon.org/wandbox/api/compile.json", true);
    http.onreadystatechange = function(){
        if (http.readyState == 4 && http.status == 200) {
            var output_json = JSON.parse(http.response);
            if ('status' in output_json && output_json.status == "0") {
                if ('program_message' in output_json) {
                    cpp_output[id].setValue(output_json.program_message);
                }
                cpp_output[id].setValue(cpp_output[id].getValue() + "\n-------\nExit: " + output_json.status);
            } else if ('compiler_error' in output_json) {
                cpp_output[id].setValue(output_json.compiler_error);
            } else if ('signal' in output_json) {
                if ('program_message' in output_json) {
                    cpp_output[id].setValue(output_json.program_message);
                }
            }
            document.getElementById("compile_and_run_" + id).firstChild.data = "Compile & Run (Ctrl+Enter)";
        }
    }

    http.send(
        JSON.stringify({
          "code" : cpp_code[id].getValue()
        , "codes" : [{
              "file" : "boost/di.hpp"
            , "code" : get_cpp_file("https://raw.githubusercontent.com/krzysztof-jusiak/di/" + document.getElementById("version_" + id).value + "/include/boost/di.hpp")
           }]
         , "options": "warning,cpp-pedantic-errors,optimize,boost-nothing,c++1y"
		 , "compiler" : document.getElementById("compiler_" + id).value
         , "compiler-option-raw": "-I." + "\n" + "-DBOOST_DI_CFG_DIAGNOSTICS_LEVEL=" + document.getElementById("log_level_" + id).value
    }));
}

function get_example(id, file) {
    cpp_code[id] = CodeMirror.fromTextArea(document.getElementById("code_" + id), {
        lineNumbers: true,
        matchBrackets: true,
        styleActiveLine: true,
        mode: "text/x-c++src"
      });

    cpp_code[id].setSize(1100, 500);
    cpp_code[id].addKeyMap({"Ctrl-Enter": function(cm){ compile_and_run(id); }});

    cpp_output[id] = CodeMirror.fromTextArea(document.getElementById("output_" + id), {
        lineNumbers: true,
        matchBrackets: true,
        styleActiveLine: true,
        readOnly : true,
        mode: "text/x-c++src"
      });

    cpp_output[id].setSize(1100, 200);
    cpp_output[id].setOption("theme", 'mdn-like');

    cpp_code[id].setValue(get_cpp_file(file));
}

Element.prototype.remove = function() {
    this.parentElement.removeChild(this);
}

NodeList.prototype.remove = HTMLCollection.prototype.remove = function() {
    for(var i = 0, len = this.length; i < len; i++) {
        if(this[i] && this[i].parentElement) {
            this[i].parentElement.removeChild(this[i]);
        }
    }
}

$(document).ready(function () {
    $('img[alt="CPP"]').each(function () {
        var id = $(this).attr('src');
        var r='<button id="run_it_btn_1" onclick="show(1, \''+id+'\')">Run this code!</button><textarea style="display: none" id="code_1"></textarea><br /><textarea style="display: none" id="output_1"></textarea><div id="code_listing_1">blah</div>';
    console.log("b");
        $(this).replaceWith(r);
    console.log("c");
    });
});

