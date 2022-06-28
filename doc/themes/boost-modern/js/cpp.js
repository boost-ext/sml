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
    compile_btn.setAttribute("style", "position: relative; top: 60px; right: 20px; z-index: 10;");
    compile_btn.appendChild(compile_txt);
    compile_btn.onclick = function() { compile_and_run(id) };

    document.getElementById("run_it_btn_" + id).parentNode.insertBefore(compile_btn, document.getElementById("run_it_btn_" + id).nextSibling);

    get_example(id, file);
    compile_and_run(id);
}

function compile_and_run(id) {
    document.getElementById("compile_and_run_" + id).firstChild.data = "Compiling...";
    cpp_output[id].setValue("Compiling...");
    var http = new XMLHttpRequest();
    http.open("POST", "http://melpon.org/wandbox/api/compile.json", true);
    http.onreadystatechange = function(){
        if (http.readyState == 4 && http.status == 200) {
            var output_json = JSON.parse(http.response);
            if ('status' in output_json && output_json.status == "0") {
                cpp_output[id].setValue("Running... [OK]");
                if ('program_message' in output_json) {
                    cpp_output[id].setValue(cpp_output[id].getValue() + "\n" + output_json.program_message);
                }
            } else if ('compiler_error' in output_json) {
                cpp_output[id].setValue("Compiling... [FAIL]\n" + output_json.compiler_error);
            } else if ('signal' in output_json) {
                if ('program_message' in output_json) {
                    cpp_output[id].setValue("Running... [FAIL]\n" + output_json.program_message);
                }
            }
            document.getElementById("compile_and_run_" + id).firstChild.data = "Compile & Run (Ctrl+Enter)";
        }
    }

    http.send(
        JSON.stringify({
          "code" : cpp_code[id].getValue()
        , "codes" : [{
              "file" : "boost/sml.hpp"
            , "code" : get_cpp_file("https://raw.githubusercontent.com/boost-ext/sml/master/include/boost/sml.hpp")
           }]
         , "options": "warning,cpp-pedantic-errors,optimize,boost-1.60,c++1y"
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
    cpp_code[id].setCursor(Math.floor(cpp_code[id].lineCount() - (cpp_code[id].lineCount() / 3)) , 0);
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

function cleanup(str) {
  text = str.replace(/[\n]{3,}/g, "\n").slice(0, -1);
  var lines = text.split('\n');
  var result = '';
  for(var i = 0; i < lines.length; i++) {
    if (!i && !lines[i].length) {
        continue;
    }
    result += lines[i] + "\n";
  }
  return result;
}

$(document).ready(function () {
    $('img[alt="CPP"]').each(function () {
      var file = $(this).attr('src');
      var basename = $(this).attr('src').split('/')[$(this).attr('src').split('/').length - 1];
      var begin = "//<-";
      var end = "//->";
      var example = get_cpp_file(file);
      var lines = example.split('\n');
      var example_result = '';
      var ignored = false;
      var tmp_ignored = false;
      for(var i = 0; i < lines.length; ++i){
        var line = lines[i];
        if (line.indexOf(begin) != -1) {
          ignored = true;
          tmp_ignored = true;
        } else if (line.indexOf(end) != -1) {
          tmp_ignored = false;
        }
        if (!ignored && line.trim().indexOf("//") != 0) {
          example_result += line + '\n';
        }
        ignored = tmp_ignored;
      }
      var id = gid++;
      var compile = "\/\/ $CXX -std=c++14 " + basename;
      example = $('<div/>').text(example_result.replace(/[\n]{3,}/g, "\n")).html();
      $(this).replaceWith('<button style="position: relative; top: 60px; right: 20px; z-index: 10;" class="btn btn-neutral float-right" id="run_it_btn_' + id + '" onclick="cpp(' + id + ', \'' + file + '\', \'Run this code!\')">Run this code!</button><textarea style="display: none" id="code_' + id + '"></textarea><br /><textarea style="display: none" id="output_' + id + '"></textarea><div id="code_listing_' + id + '"><pre><code style="line-height: 12px; width: 100%" class="cpp hljs">' + compile + '\n' + example + '</code></pre></div>');
    });

    $('img[alt="CPP(SHOW)"]').each(function () {
      var file = $(this).attr('src');
      var basename = $(this).attr('src').split('/')[$(this).attr('src').split('/').length - 1];
      var begin = "//<-";
      var end = "//->";
      var example = get_cpp_file(file);
      var lines = example.split('\n');
      var example_result = '';
      var ignored = false;
      var tmp_ignored = false;
      for(var i = 0; i < lines.length; ++i){
        var line = lines[i];
        if (line.indexOf(begin) != -1) {
          ignored = true;
          tmp_ignored = true;
        } else if (line.indexOf(end) != -1) {
          tmp_ignored = false;
        }
        if (!ignored && line.trim().indexOf("//") != 0) {
          example_result += line + '\n';
        }
        ignored = tmp_ignored;
      }
      var id = gid++;
      example = $('<div/>').text(example_result.replace(/[\n]{3,}/g, "\n")).html();
      $(this).replaceWith('<pre><code style="line-height: 12px; font-size: 90%; width: 100%" class="cpp hljs">' + example + '</code></pre>');
    });

    $('img[alt="CPP(BTN)"]').each(function () {
      var text = $(this).attr('src');
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

    $('img[alt="CPP(SPLIT)"]').each(function () {
      var file = $(this).attr('src');
      var basename = $(this).attr('src').split('/')[$(this).attr('src').split('/').length - 1];
      var name = basename.replace(".cpp", "").replace(/_/g, " ");
      name = name.charAt(0).toUpperCase() + name.substring(1);
      var begin = "//<-";
      var end = "//->";
      var example = get_cpp_file(file);
      var lines = example.split('\n');
      var example_result = '';
      var test_result = '';
      var ignored = 0;
      var tmp_ignored = 0;
      for(var i = 0; i < lines.length; i++){
        var line = lines[i];
        if (line.indexOf(begin) != -1) {
          ignored = 1;
          tmp_ignored = 1;
        } else if (line.indexOf(end) != -1) {
          tmp_ignored = 0;
        }

        if (!ignored && line.indexOf("/**") == 0) {
            test_result += line + '\n';
            tmp_ignored = 2;
            ignored = 1;
        }
        else if (!ignored && line.indexOf("int main()") == 0) {
          if (example_result.replace(/\n/g, "") == "") {
            tmp_ignored = 3;
          } else {
            tmp_ignored = 2;
          }
          ignored = 1;
        } else if (ignored == 3 && !line.length) {
          tmp_ignored = 2;
          ignored = 1;
        }
        if (line.trim().indexOf("//") != 0) {
          if (!ignored) {
            example_result += line + '\n';
          } else if (ignored == 2 && line.indexOf("}") != 0) {
            test_result += line + '\n';
          } else if (ignored == 3) {
            example_result += line + '\n';
          }
        }
        ignored = tmp_ignored;
      }
      var id = gid++;
      var ex = cleanup(example_result);
      var ts = cleanup(test_result);
      var height = Math.max(ex.split("\n").length, ts.split("\n").length) * 13;
      example = $('<div/>').text(ex).html();
      test = $('<div/>').text(ts).html();
      $(this).replaceWith('<table style="table-layout: fixed; border-collapse:collapse; padding:0; height: 30px; width: 100%; border: 1px;"><thead><tr><th>' + name + '</th><th><button class="btn btn-neutral" id="run_it_btn_' + id + '" onclick="cpp(' + id + ', \'' + file + '\', \'Test this code!\')">Test this code!</button><div id="code_listing_' + id + '"></th></tr></thead><tbody><tr><td><pre><code class="cpp hljs" style="line-height: 12px; height: ' + height + 'px;">' + example + '</code></pre></td><td><pre><code class="cpp hljs" style="line-height: 12px; height: ' + height + 'px;">' + test + '</code></pre></td></tr></tbody></table><textarea style="display: none" id="code_' + id + '"></textarea><br /><textarea style="display: none" id="output_' + id + '"></textarea></div>');
    });
});
