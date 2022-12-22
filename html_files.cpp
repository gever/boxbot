/**********************************************************
DO NOT EDIT - BUILT BY THE BUILD SCRIPT: https://replit.com/@gever/bbdebuggui
*************************************************************/ 
const char *script_js= R"VERBATIM(

// immediate motion 
function boxbot_move(v) {
  fetch('/move?a=' + v); // .then((response) => console.log(response.json()));
}
function boxbot_turn(v) {
  fetch('/turn?a=' + v); // .then((response) => console.log(response.json()));
}
function boxbot_stop(v) {
  fetch('/stop'); // .then((response) => console.log(response.json()));
}
function boxbot_send_plan(plan) {
  fetch('/plan?a=' + plan); //.then((response) => console.log(response.json()));
  console.log('plan: ' + plan);
}

// motion planning support
// add click handlers to all of the command panel buttons
const buttons = document.querySelectorAll('.panel_button');
const lines_of_code = document.querySelectorAll("#lines_of_code")[0];

// TODO: consider adding a BOOP noise
function confirmationAnimation(elem) {
  var original_color = elem.style.backgroundColor;
  elem.style.backgroundColor = "white";
  setTimeout(function() {
    elem.style.backgroundColor = original_color;
  }, 150);
}

function addLineOfCode(evt, id, bg_color) {
  if (lines_of_code.childElementCount <= 9) {
    confirmationAnimation(evt.target);
    var elem = document.createElement("pre");
    // translate the button id into code
    elem.innerHTML = id.split('_').join(' ');
    elem.style.backgroundColor = bg_color;
    lines_of_code.appendChild(elem);
  } else {
    console.log("line limit");
    console.log(lines_of_code.length);
  }
}

function deleteTheCode() {
  while (lines_of_code.firstChild) {
    lines_of_code.removeChild(lines_of_code.firstChild);
  }
}
function clearCode(evt) {
  confirmationAnimation(evt.target);
  deleteTheCode();
}

function undoCode(evt) {
  confirmationAnimation(evt.target);
  if (lines_of_code.lastChild) {
    lines_of_code.removeChild(lines_of_code.lastChild);
  }
}

// encode the "motion plan" in a dense format that is
// easier to parse on the boxbot (until python/lisp are working)
// input:
//   MOV,FWD,10
//   MOV,BWD,10
//   TRN,LT,90
//   TRN,RT,90
//   TRN,LT,30
//   TRN,RT,30
//   MOV,FWD,5
//   MOV,BWD,5
//   TRN,LT,180
//   TRN,RT,180
//  output:
//   M10,M-10,T-90,T90,T-30,T30,M-5,M5,T-180,T180
function boxbot_translate(raw_code) {
  var code = raw_code.split("\n");
  var output = "";
  for (var i = 0; i < code.length; i++) {
    var line = code[i];
    if (line.length == 0) continue;

    var parts = line.split(",");
    var opcode = parts[0];
    var modifier = parts[1];
    var param = parts[2];
    if (output.length > 0) {
      output += ",";
    }
    if (opcode == "TRN") {
      output += "T";
    } else if (opcode == "MOV") {
      output += "M";
    } else {
      return "ERROR: unknown opcode: " + opcode;
    }
    if ((modifier == "LT") || (modifier == "BWD")) {
      output += "-";
    }
    output += param;
  }
  return output;
}

var last_send = Date.now();
function sendCode(evt) {
  if ((Date.now() - last_send) < 1000) {
    // don't let them pound the send button
    return;
  }
  last_send = Date.now();

  // collect the code
  confirmationAnimation(evt.target);
  var current = lines_of_code.firstChild;
  var code_blob = "";
  while (current) {
    code_blob += current.innerHTML + "\n";
    current = current.nextSibling;
  }
  code_blob = code_blob.split(' ').join(',');
  var translated = boxbot_translate(code_blob);
  if (translated.startsWith("ERROR")) {

  } else {
    boxbot_send_plan(translated);
    // console.log("Code---");
    // console.log(code_blob);
    // window.BU_send( code_blob );
    setTimeout(deleteTheCode, 500);
  }
}

function forceRefresh() {
  window.location.reload();
}

// add the click handlers to the buttons
buttons.forEach(box => {
  box.addEventListener('click', function handleClick(event) {
    addLineOfCode(event, box.id, window.getComputedStyle(box).backgroundColor);
  });
});


)VERBATIM";

const char *style_css= R"VERBATIM(
html, body {
  font-family: sans-serif;
  height: 100%;
  width: 100%;
  background-color: #000000;
}

.banner {
  font-family: sans-serif;
  font-weight: bold;
  color: #28b4f0;
  background-color: #424242;
}

.button_body {
  border-radius: 5px;
  border-style: solid;
  border-width: 2px;
  cursor: pointer;
  padding: 20px;

  -webkit-touch-callout: none;
  -webkit-user-select: none;
  -khtml-user-select: none;
  -moz-user-select: none;
  -ms-user-select: none;
  user-select: none;
}

.arrow {
  font-size: 30px;
  font-weight: bold;
}

.unselectable {
    -webkit-touch-callout: none;
    -webkit-user-select: none;
    -khtml-user-select: none;
    -moz-user-select: none;
    -ms-user-select: none;
    user-select: none;
}

.formbuilder-group-label {
  font-weight: bold;
  font-size: large;
}
.tooltip-element {
  font-style: italic;
  font-size: small;
}
.form-group {
  margin-bottom: 20px;
}

/* motion planning styles */
body {
  margin: 0px;
  /*
  width: 1024px;
  height: 768px;
  */
}

td {
  vertical-align: top;
}

.container {
  width: 100%;
  height: 100%;
}

.command_container {
  width: 800px;
  height: 200px;
}

.code_container {
  width: 400px;
  height: 100%;
}

.panel_button {
  padding: 0px;
  width: 120px;
  height: 80px;
  text-align: center;
  font-family: 'Gill Sans', 'Gill Sans MT', Calibri, 'Trebuchet MS', sans-serif;
  font-weight: bold;
  font-size: 20px;
  background-color: white;
  border-radius: 10px;
  /* disable text selection */
  -webkit-user-select: none;
  user-select: none;
}

.panel_title {
  font-family: 'Gill Sans', 'Gill Sans MT', Calibri, 'Trebuchet MS', sans-serif;
  font-weight: bold;
  font-size: 30px;
  text-align: left;
  width: 100%;
  background-color: rgb(175, 196, 223);
}

.code_button {
  padding: 0px;
  height: 70px;
  width: 100px;
  text-align: center;
  font-family: 'Gill Sans', 'Gill Sans MT', Calibri, 'Trebuchet MS', sans-serif;
  font-weight: bold;
  font-size: 20px;
  background-color: rgb(51, 204, 84);
  border-radius: 10px;
  /* disable text selection */
  -webkit-user-select: none;
  user-select: none;
}

.clear_plan_button {
  background-color: orange;
}

.undo_button {
  background-color: #ff9500;
}

.button_contents {
  padding-top: 28%;
}

.code_button_contents {
  color: white;
  padding-top: 20%;
}

.fwd_style {
  background-color: yellow;
}

.bwd_style {
  background-color: cyan;
}

.right_style {
  background-color: #FF914A;
}

.left_style {
  background-color: #7FA1FF; 
}

.latest_image {
  width: 800px;
  height: 600px;
  /* background-color: rgb(142, 248, 151); */
  /* background-image: url("latest_image.jpg"); */
}
.instructions {
  background-color: purple;
}

#lines_of_code {
  /* height: 485px; */
  height: 250px;
  padding-left: 10px;
}

#coms_status {
}

pre {
  margin: 2px;
  font-size: 20px;
}
)VERBATIM";

const char *touch_go_html= R"VERBATIM(
<!DOCTYPE html>
<html>

<head>
  <meta charset="utf-8">
  <meta name="viewport" content="width=device-width">
  <title>Boxbot 0.1</title>
  <link href="style.css" rel="stylesheet" type="text/css" />
</head>

<body>
  <table>
    <tr>
      <td class="button_body" onclick="boxbot_move(1)">FWD 1</td>
      <td class="button_body" onclick="boxbot_move(-1)">BWD 1</td>
      <td class="button_body" onclick="boxbot_turn(-15)">LT 15</td>
      <td class="button_body" onclick="boxbot_turn(15)">
        <div class="arrow">&#10227;</div>
        <div>15</div>
      </td>
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

  <a href="settings.html">settings</a>
  <script src="script.js"></script>
</body>

</html>
)VERBATIM";

const char *settings_html= R"VERBATIM(
<!DOCTYPE html>
<html>

<head>
  <meta charset="utf-8">
  <meta name="viewport" content="width=device-width">
  <title>Boxbot Settings</title>
  <link href="style.css" rel="stylesheet" type="text/css" />
</head>

<body>
  <div class="rendered-form">
    <div class="">
      <h1 access="false" id="control-1770517">Boxbot System Settings</h1>
    </div>
    <form action="/save" method="post" class="settings-form">
      <div class="formbuilder-radio-group form-group">
        <div class="formbuilder-group-label">Set Orientation</div>
        <div class="tooltip-element">Determines which direction the boxbot goes when asked to go forward.</div>
        <div class="radio-group">
          <div class="formbuilder-radio">
            <input name="radio-group-1669745857659" access="false" id="radio-group-1669745857659-0"
              value="wheels-in-front" type="radio" checked="checked">
            <label for="radio-group-1669745857659-0">Wheels in Front</label>
          </div>
          <div class="formbuilder-radio">
            <input name="radio-group-1669745857659" access="false" id="radio-group-1669745857659-1"
              value="wheels-in-back" type="radio">
            <label for="radio-group-1669745857659-1">Wheels in Back</label>
          </div>
        </div>
      </div>
      <div class="form-group">
        <div class="formbuilder-group-label">Step Rate</div>
        <div class="tooltip-element">Sets the delay between motor steps. Less than 950 reduces torque.</div>
        <input type="number" class="form-control" name="motor-step-rate" access="false" value="950" min="100"
          max="10000" id="motor-step-rate">
      </div>
      <div class="form-group">
        <div class="formbuilder-group-label">Linear Turn Fudge</div>
        <div class="tooltip-element">This constant is multiplied by the calculated "idealized" turn result
          (which is based on a 80mm diameter and zero-slip surface) to correct under/over turnage. More
          than 1.0 creates more turn, less than 1.0 creates less turn.</div>
        <input type="text" class="form-control" name="linear-turn-fudge" value="1.0" id="linear-turn-fudge">
      </div>
      <div class="form-group">
        <div class="formbuilder-group-label">Linear Motion Fudge</div>
        <div class="tooltip-element">This constant is multiplied by the calculated "idealized" movement result
          (which is based on a 80mm diameter and zero-slip surface) to correct under/over movement. More
          than 1.0 creates more movement, less than 1.0 creates less movement.</div>
        <input type="text" class="form-control" name="linear-motion-fudge" value="1.0" id="linear-motion-fudge">
      </div>
      <div class="formbuilder-button form-group">
        <button type="submit" class="btn-success btn" name="save-button" access="false" style="success"
          id="save-button">Save</button>
      </div>
    </form>
  </div>
  <script src="script.js"></script>
</body>

</html>
)VERBATIM";

const char *index_html= R"VERBATIM(
<!DOCTYPE html>
<html>
<!-- MOTION PLAN GUI -->

<head>
  <meta charset="utf-8">
  <meta name="viewport" content="width=device-width">
  <title>boxbot</title>
  <link href="style.css" rel="stylesheet" type="text/css" />
</head>

<body style="width:100%;">
  <div class="banner">Chabot Space and Science Center presents: Tinkering School Boxbot Beta</div>
  <table style="width:100%">
    <tr>
      <td class="command_container">
        <table width="100%;">
          <!-- first row -->
          <tr>
            <td id="MOV_FWD_1" class="panel_button fwd_style">
              <div class="button_contents">Forward 1</div>
            </td>
            <td id="MOV_BWD_1" class="panel_button bwd_style">
              <div class="button_contents">Backward 1</div>
            </td>
            <td id="TRN_LT_15" class="panel_button left_style">
              <div class="button_contents">Left 15</div>
            </td>
            <td id="TRN_RT_15" class="panel_button right_style">
              <div class="button_contents">Right 15</div>
            </td>
          </tr>

          <!-- second row -->
          <tr>
            <td id="MOV_FWD_5" class="panel_button fwd_style">
              <div class="button_contents">Forward 5</div>
            </td>
            <td id="MOV_BWD_5" class="panel_button bwd_style">
              <div class="button_contents">Backward 5</div>
            </td>
            <td id="TRN_LT_30" class="panel_button left_style">
              <div class="button_contents">Left 30</div>
            </td>
            <td id="TRN_RT_30" class="panel_button right_style">
              <div class="button_contents">Right 30</div>
            </td>
          </tr>

          <!-- third row -->
          <tr>
            <td id="MOV_FWD_10" class="panel_button fwd_style">
              <div class="button_contents">Forward 10</div>
            </td>
            <td id="MOV_BWD_10" class="panel_button bwd_style">
              <div class="button_contents">Backward 10</div>
            </td>
            <td id="TRN_LT_90" class="panel_button left_style">
              <div class="button_contents">Left 90</div>
            </td>
            <td id="TRN_RT_90" class="panel_button right_style">
              <div class="button_contents">Right 90</div>
            </td>
          </tr>

          <!-- fourth row -->
          <tr>
            <td id="MOV_FWD_20" class="panel_button fwd_style">
              <div class="button_contents">Forward 20</div>
            </td>
            <td id="MOV_BWD_20" class="panel_button bwd_style">
              <div class="button_contents">Backward 20</div>
            </td>
            <td id="TRN_LT_180" class="panel_button left_style">
              <div class="button_contents">Left 180</div>
            </td>
            <td id="TRN_RT_180" class="panel_button right_style">
              <div class="button_contents">Right 180</div>
            </td>
          </tr>
        </table>
      </td>

      <td>
        <div class="code_container">
          <div class="main_pane">
            <div class="panel_title" onclick="forceRefresh(event)">Motion Plan</div>
            <div id="lines_of_code"></div>
          </div>
          <table width="100%" style="vertical-align:bottom;">
            <tr>
              <td>
                <div id="button_undo" class="code_button undo_button" onclick="undoCode(event)">
                  <div class="code_button_contents">Undo</div>
                </div>
              </td>
              <td>
                <div id="button_clear" class="code_button clear_plan_button" onclick="clearCode(event)">
                  <div class="code_button_contents">Clear Plan</div>
                </div>
              </td>
              <td>
                <div id="button_send" class="code_button" onclick="sendCode(event)">
                  <div class="code_button_contents">UPLINK</div>
                </div>
              </td>
            </tr>
          </table>
        </div>
      </td>
    </tr>
  </table>
  <script src="script.js"></script>
</body>

</html>
)VERBATIM";
