<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />

<script type="text/javascript">
var xmlhttp;
if (window.XMLHttpRequest) {
  // code for IE7+, Firefox, Chrome, Opera, Safari
  xmlhttp=new XMLHttpRequest();
} else {
  // code for IE6, IE5
  xmlhttp=new ActiveXObject("Microsoft.XMLHTTP");
}

function addRow(jsonobj) {
    var tbl = document.getElementById("tbl");
    var newrow = tbl.insertRow(0); // default, -1, insert to last row
    var newcell = newrow.insertCell();
    newcell.innerHTML = jsonobj.id;
    var newcell = newrow.insertCell();
    newcell.innerHTML = jsonobj.task;
}
function newItems() {
    if (xmlhttp.readyState == 4 && xmlhttp.status == 200) {
        console.log('data: |' + xmlhttp.responseText + '|');
        var jsonobj = JSON.parse(xmlhttp.responseText);
        console.log('test');
        var output = xmlhttp.responseText;
        document.getElementById("raw").innerHTML = output;
        addRow(jsonobj);
        document.getElementById("newtask").value = "";
    } else {
        alert("no data");
    }
}
function sendData()
{
    xmlhttp.onload = newItems;
    var task=document.getElementById("newtask").value;
    console.log("input text: " + task);
    var uri="/new?task=" + task;
    xmlhttp.open("GET", uri, true);
    xmlhttp.send();
}
var form = document.getElementById("newtaskform");


// form.addEventListener("submit", function (event) {
//         event.preventDefault();
//
//         sendData();
//         });
//
</script>


</head>
<body>
<p> add a new task to ToDo list:<p>
<form action="/new" method="GET" id="newtaskform">
<input type="text" size="100" maxlength="100" name="task" id="newtask">
</form>
<button type="button" onclick="sendData()">Click Me!</button>


<p>The open items are as follows:</p>
<table border="1" id="tbl">
%for row in rows:
  <tr>
  %for col in row:
    <td>{{col}}</td>
  %end
  </tr>
%end
</table>

  <p>The raw result from the ajax json request is:</p>
  <div id="raw"></div>
  <br />
  <p>The for cycle produces :</p>
  <div id="forin"></div>
</body>
</html>
