<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />

<script type="text/javascript">
var xmlhttp;

// Are we using a modern browser or ...
if (window.XMLHttpRequest) {
  // code for IE7+, Firefox, Chrome, Opera, Safari
  xmlhttp=new XMLHttpRequest();
} else {
  // code for IE6, IE5
  xmlhttp=new ActiveXObject("Microsoft.XMLHTTP");
}

// This will render the two output which substitute the
// elements id="raw" and id="forin"
function GetItems()
{
  if (xmlhttp.readyState==4 && xmlhttp.status==200) {
    var jsonobj = JSON.parse(xmlhttp.responseText); 
    var output = xmlhttp.responseText;
    document.getElementById("raw").innerHTML = output;
    output = "";
    for (i in jsonobj) {
      output += '<p>';
      output += i + " : " + jsonobj[i];
      output += '</p>';
    }
    document.getElementById("forin").innerHTML = output;
  } else {
    alert("data not available");
  }
}

// xmlhttp.onreadystatechange = GetArticles;
// the GetItems function will be triggered once the ajax
// request is terminated.

// send the request in an async way
function sendData() {
xmlhttp.onload = GetItems;
    xmlhttp.open("GET", "/getallitems.json", true);
    xmlhttp.send();
    // GetItems();
}
</script>
</head>

<body>
  <button type="button" onclick="sendData({test:'ok'})">Click Me!</button>
  <p>The raw result from the ajax json request is:</p>
  <div id="raw"></div>
  <br />
  <p>The for cycle produces :</p>
  <div id="forin"></div>
</body>
</html>
