var cpp_code = Array();
var cpp_output = Array();
var gid = 0;

function get_cpp_file(file) {
     var cpp_file = new XMLHttpRequest();
     cpp_file.open("GET", file, false);
     cpp_file.send();
     return cpp_file.responseText;
}

$.fn.toHtml=function(){
   return $(this).html($(this).text())
}

function toggle(id, file, text) {
    document.getElementById("run_it_btn_" + id).firstChild.data = text;
    document.getElementById("run_it_btn_" + id).removeChild;
    document.getElementById("run_it_btn_" + id).onclick = function() { cpp(id, file, text); };
    document.getElementById("code_listing_" + id).style.display = 'block';
    cpp_code[id].toTextArea();
    document.getElementById("code_" + id).style.display = 'none';
    cpp_output[id].toTextArea();
    document.getElementById("output_" + id).style.display = 'none';
    document.getElementById("compile_and_run_" + id).remove();
}

function cpp(id, file, text) {
    document.getElementById('run_it_btn_' + id).firstChild.data = 'Exit';
    document.getElementById("run_it_btn_" + id).onclick = function() { toggle(id, file, text); };
    document.getElementById("code_listing_" + id).style.display = 'none';

    var compile_btn = document.createElement("BUTTON");
    var compile_txt = document.createTextNode("Compile & Run (Ctrl+Enter)");
    compile_btn.setAttribute("id", "compile_and_run_" + id);
    compile_btn.setAttribute("class", "btn btn-neutral float-right");
    compile_btn.appendChild(compile_txt);
    compile_btn.onclick = function() { compile_and_run(id) };

    document.getElementById("run_it_btn_" + id).parentNode.insertBefore(compile_btn, document.getElementById("run_it_btn_" + id).nextSibling);

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
              "file" : "boost/msm-lite.hpp"
            , "code" : get_cpp_file("https://raw.githubusercontent.com/boost-experimental/msm-lite/master/include/boost/msm-lite.hpp")
           }]
       , "options": "warning,cpp-pedantic-errors,optimize,boost-nothing,c++1y"
       , "compiler" : "clang-head"
       , "compiler-option-raw": "-I." + "\n" + "-fno-color-diagnostics"
    }));
}

function get_example(id, file) {
    cpp_code[id] = CodeMirror.fromTextArea(document.getElementById("code_" + id), {
        lineNumbers: true,
        matchBrackets: true,
        styleActiveLine: true,
        mode: "text/x-c++src"
      });

    cpp_code[id].setSize(930, 500);
    cpp_code[id].addKeyMap({"Ctrl-Enter": function(cm){ compile_and_run(id); }});

    cpp_output[id] = CodeMirror.fromTextArea(document.getElementById("output_" + id), {
        lineNumbers: true,
        matchBrackets: true,
        styleActiveLine: true,
        readOnly : true,
        mode: "text/x-c++src"
      });

    cpp_output[id].setSize(930, 150);
    cpp_output[id].setOption("theme", 'mdn-like');

    cpp_code[id].setValue(get_cpp_file(file));
	  cpp_code[id].setCursor(cpp_code[id].lineCount() / 2, 0);
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
        var file = $(this).attr('src');
        var basename = $(this).attr('src').split('/')[$(this).attr('src').split('/').length - 1];
		var regex = "#include.*";
		var example = get_cpp_file(file);
		var i = example.lastIndexOf("#include")
		var n = example.substring(i).indexOf('\n');
		var id = gid++;
		var compile = "\/\/ $CXX -std=c++14 " + basename;
		example = $('<div/>').text(example.substring(i + n + 2)).html();
        $(this).replaceWith('<button class="btn btn-neutral float-right" id="run_it_btn_' + id + '" onclick="cpp(' + id + ', \'' + file + '\', \'Run this code!\')">Run this code!</button><textarea style="display: none" id="code_' + id + '"></textarea><br /><textarea style="display: none" id="output_' + id + '"></textarea><div id="code_listing_' + id + '"><pre><code class="cpp">' + compile + '\n\n' + example + '</code></pre></div>');
    });

    $('img[alt="CPP(BTN)"]').each(function () {
        var text = $(this).attr('src');
        console.log(text);
        var name = text.split("|")[0].replace(/_/g, ' ').replace(/\//g, '').replace(/\./g, '');
        var file = text.split("|")[1];
        var basename = $(this).attr('src').split('/')[$(this).attr('src').split('/').length - 1];
		var regex = "#include.*";
		var example = get_cpp_file(file);
		var i = example.lastIndexOf("#include")
		var n = example.substring(i).indexOf('\n');
		var id = gid++;
		var compile = "\/\/ $CXX -std=c++14 " + basename;
		example = $('<div/>').text(example.substring(i + n + 2)).html();
        $(this).replaceWith('<table class="float-left"><tr><td><button class="btn" id="run_it_btn_' + id + '" onclick="cpp(' + id + ', \'' + file + '\', \'' + name + '\')">' + name + '</button><textarea style="display: none" id="code_' + id + '"></textarea><br /><textarea style="display: none" id="output_' + id + '"></textarea><div style=""id="code_listing_' + id + '"></div></td></tr></table>');
    });
});
