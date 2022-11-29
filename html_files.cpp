const char *script_js = R"VERBATIM(
function boxbot_move(v) {
  fetch('/move?a=' + v); // .then((response) => console.log(response.json()));
}
function boxbot_turn(v) {
  fetch('/turn?a=' + v); // .then((response) => console.log(response.json()));
}
function boxbot_stop(v) {
  fetch('/stop'); // .then((response) => console.log(response.json()));
}
)VERBATIM";

const char *style_css = R"VERBATIM(
html, body {
  height: 100%;
  width: 100%;
}

.button_body {
  border-radius: 5px;
  border-style: solid;
  border-width: 2px;
  cursor: pointer;

  -webkit-touch-callout: none;
  -webkit-user-select: none;
  -khtml-user-select: none;
  -moz-user-select: none;
  -ms-user-select: none;
  user-select: none;
}

.unselectable {
    -webkit-touch-callout: none;
    -webkit-user-select: none;
    -khtml-user-select: none;
    -moz-user-select: none;
    -ms-user-select: none;
    user-select: none;
}
)VERBATIM";

const char *index_html = R"VERBATIM(
<!DOCTYPE html>
<html>

<head>
  <meta charset="utf-8">
  <meta name="viewport" content="width=device-width">
  <title>replit</title>
  <link href="style.css" rel="stylesheet" type="text/css" />
</head>

<body>
  <table>
    <tr>
      <td class="button_body" onclick="boxbot_move(1)">FWD 1</td>
      <td class="button_body" onclick="boxbot_move(-1)">BWD 1</td>
      <td class="button_body" onclick="boxbot_turn(-15)">LT 15</td>
      <td class="button_body" onclick="boxbot_turn(15)">RT 15</td>
    </tr>
    <tr>
      <td class="button_body" onclick="boxbot_move(5)">FWD 5</td>
      <td class="button_body" onclick="boxbot_move(-5)">BWD 5</td>
      <td class="button_body" onclick="boxbot_turn(-45)">LT 45</td>
      <td class="button_body" onclick="boxbot_turn(45)">RT 45</td>
    </tr>
    <tr>
      <td class="button_body" onclick="boxbot_move(10)">FWD 10</td>
      <td class="button_body" onclick="boxbot_move(-10)">BWD 10</td>
      <td class="button_body" onclick="boxbot_turn(-90)">LT 90</td>
      <td class="button_body" onclick="boxbot_turn(90)">RT 90</td>
    </tr>
    <tr>
      <td colspan=4 class="button_body" style="text-align:center" onclick="boxbot_stop()">stop</td>
    </tr>
  </table>
  <script src="script.js"></script>
</body>

</html>
)VERBATIM";

const char *setup_html = R"VERBATIM(
)VERBATIM";
