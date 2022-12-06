const char *index_html= R"VERBATIM(
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

  <a href="settings.html">settings</a>
  <script src="script.js"></script>
</body>

</html>
)VERBATIM";

const char *script_js= R"VERBATIM(
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
        <input type="number" class="form-control" name="linear-turn-fudge" access="false" value="1.0"
          id="linear-turn-fudge">
      </div>
      <div class="form-group">
        <div class="formbuilder-group-label">Linear Motion Fudge</div>
        <div class="tooltip-element">This constant is multiplied by the calculated "idealized" movement result
          (which is based on a 80mm diameter and zero-slip surface) to correct under/over movement. More
          than 1.0 creates more movement, less than 1.0 creates less movement.</div>
        <input type="number" class="form-control" name="linear-motion-fudge" access="false" value="1.0"
          id="linear-turn-fudge">
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

const char *style_css= R"VERBATIM(
html, body {
  font-family: sans-serif;
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
)VERBATIM";
