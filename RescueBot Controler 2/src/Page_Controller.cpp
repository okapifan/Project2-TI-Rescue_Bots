#include <avr/pgmspace.h>

const char* page_Controller PROGMEM = R"=====(
<!DOCTYPE html>
<html>

<head>
	<title>RescueBot 2</title>
	<link rel='icon' type='image/png' href="data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAACAAAAAgCAYAAABzenr0AAAAAXNSR0IArs4c6QAAAARnQU1BAACxjwv8YQUAAAAJcEhZcwAADsMAAA7DAcdvqGQAAAE7SURBVFhH7ZWxCsIwEIar4OTg1Bdw9l2Ejs5CQfAB3Lr5AIJQcHYs+C7OvkAnByeHSq49SeIld22VOPSDcmnTu9z9uZBRVVVRSMaNDcaQwJCA8xQkSWJMFEUxaoYipP6kArazgvrmoo0/mUAcx+QjhfJVD8VHAmmaOiv1zSFt/Y0esH/IssywSFmWzcjErvK6PIBdXLZgkTzP3/3wX6cAFbArRvA7p8BxXcfcnOpC99Mn2N1jApZUQLK/30Jf660AlUDfHpiv7mBv5xlYHVShUw9QR8p1zDi8Cthg5bgY9+6jtQK67NQWcPMugh9DSEAivy4rjtUFg5cMNe8D1xQpgPulAmNw/Sxz8z6gCTkFpMFsJHFZBbourpD4/rwJuSQgAddPfarXoeLgN+MyCsHPt4BjSGBIIHACUfQCT6Sg3fQBeLMAAAAASUVORK5CYII=" />
	<meta name="theme-color" content="#4286f4">

	<meta name='viewport' content='width=device-width, initial-scale=1'>
	<link href='http://fonts.googleapis.com/css?family=Roboto:400,700' rel='stylesheet' type='text/css'>
	<!--<link href='http://fonts.googleapis.com/css?family=Roboto:400,100,100italic,300,300italic,400italic,500,500italic,700,700italic,900italic,900' rel='stylesheet' type='text/css'>-->
	<style>
		p,
		h1 {
			font-family: 'Roboto', Times, serif;
		}

		#bg {
			position: fixed;
			left: 0px;
			top: 0px;
			height: 100%;
			width: 100%;
			z-index: -1;
			background: linear-gradient(to top, rgb(239, 120, 186) 0%, rgb(255, 255, 255) 40%);
		}

		.wrapper {
			width: 100%;
			clear: both;
		}

		.first {
			width: 70%;
			float: left;
			margin-top: 10px;
		}

		.second {
			width: auto;
			float: left;
			margin-left: 10px;
		}

		input.button {
			border: none;
			border-radius: 8px;
			color: black;
			text-align: center;
			font-size: 16px;
			opacity: 0.7;
		}

		input.button:hover {
			opacity: 1;
		}

		.range {
			-webkit-appearance: none;
			width: 100%;
			height: 30px;
			border-radius: 3px;
			background: #d3d3d3;
			outline: none;
			opacity: 0.7;
			-webkit-transition: .2s;
			transition: opacity .2s;
		}

		.range:hover {
			opacity: 1;
		}

		.range::-webkit-slider-thumb {
			-webkit-appearance: none;
			appearance: none;
			width: 30px;
			height: 30px;
			border-radius: 3px;
			background: #4CAF50;
			cursor: pointer;
		}

		.range::-moz-range-thumb {
			width: 30px;
			height: 30px;
			border-radius: 3px;
			background: #4CAF50;
			cursor: pointer;
		}

		#rangeR.range::-webkit-slider-thumb,
		-moz-range-thumb {
			background: #F00;
		}

		#rangeG.range::-webkit-slider-thumb,
		-moz-range-thumb {
			background: #0F0;
		}

		#rangeB.range::-webkit-slider-thumb,
		-moz-range-thumb {
			background: #00F;
		}

		#rangeW.range::-webkit-slider-thumb,
		-moz-range-thumb {
			background: #FFF;
			border: 1px solid black;
		}

		.button {
			width: 30vh;
			height: 30vh;
			font-size: 60px;
		}

		.left-section {
			position: absolute;
			left: 20vw;
		}

		.forward {
			position: fixed;
			top: 15vh;
		}

		.backward {
			position: fixed;
			bottom: 15vh;
		}

		.right-section {
			position: absolute;
			right: 10vw;
			top: 35vh;
		}

		.left {
			position: fixed;
			right: 40vw;
		}

		.right {
			position: fixed;
			right: 20vw;
		}

		.info {
			display: block;
			margin: 0;
			padding: 0;
			height: 10vh;
			text-align: center;
			padding: 10px;
		}

		html,
		body {
			margin: 0;
			padding: 0;
		}

		/* The switch - the box around the slider */
		.switch {
			position: relative;
			display: inline-block;
			width: 120px;
			height: 68px;
		}

		/* Hide default HTML checkbox */
		.switch input {
			opacity: 0;
			width: 0;
			height: 0;
		}

		/* The slider */
		.slider {
			position: absolute;
			cursor: pointer;
			top: 0;
			left: 0;
			right: 0;
			bottom: 0;
			background-color: #ccc;
			-webkit-transition: .4s;
			transition: .4s;
		}

		.slider:before {
			position: absolute;
			content: "";
			height: 52px;
			width: 52px;
			left: 8px;
			bottom: 8px;
			background-color: white;
			-webkit-transition: .4s;
			transition: .4s;
		}

		input:checked+.slider {
			background-color: #2196F3;
		}

		input:focus+.slider {
			box-shadow: 0 0 1px #2196F3;
		}

		input:checked+.slider:before {
			-webkit-transform: translateX(52px);
			-ms-transform: translateX(52px);
			transform: translateX(52px);
		}

		/* Rounded sliders */
		.slider.round {
			border-radius: 68px;
		}

		.slider.round:before {
			border-radius: 50%;
		}

		.switch-div {
			position: absolute;
			top: 1vh;
			right: 1vw;
		}
	</style>
</head>



<body>
	<div id="bg"></div>
	<div class="info">
		<Strong>RescueBot</Strong>
	</div>
	<div class="container">
		<div class="left-section">
			<button class="button forward"><strong>^</strong></button>
			<button class="button backward"><strong>v</strong></button>
		</div>
		<div class="right-section">
			<button class="button left"><strong><</strong> </button>
			<button class="button right"><strong>></strong></button>
		</div>
		<div class="switch-div">
			<i>Auto-Mode.</i>
			<label class="switch">
				<input class="autodrive" type="checkbox">
				<span class="slider round"></span>
			</label>
		</div>
	</div>
	<!--<div class='wrapper'>
    <div class='first'>
      <input id='rangeW' class='range' type='range' min='1' max='10' value='1'>
    </div>
    <div class='second'>
      <p>White: <span id='textW'></span></p>
    </div>
  </div>-->
	<script>
		//var url_string = window.location.href;
		//var url = new URL(url_string);
		//var id = url.searchParams.get("id");
		//console.log("Id: " + id);

		var bForward = false;
		var bBackward = false;
		var bLeft = false;
		var bRight = false;
		var autoDrive = false;

		var buttonForward = document.getElementsByClassName('forward')[0];
		var buttonBackward = document.getElementsByClassName('backward')[0];
		var buttonLeft = document.getElementsByClassName('left')[0];
		var buttonRight = document.getElementsByClassName('right')[0];
		var checkboxAutodrive = document.getElementsByClassName('autodrive')[0];


		buttonForward.addEventListener("touchstart", function () { bForward = true; SendData(); });
		buttonBackward.addEventListener("touchstart", function () { bBackward = true; SendData(); });
		buttonLeft.addEventListener("touchstart", function () { bLeft = true; SendData(); });
		buttonRight.addEventListener("touchstart", function () { bRight = true; SendData(); });

		buttonForward.addEventListener("mousedown", function () { bForward = true; SendData(); });
		buttonBackward.addEventListener("mousedown", function () { bBackward = true; SendData(); });
		buttonLeft.addEventListener("mousedown", function () { bLeft = true; SendData(); });
		buttonRight.addEventListener("mousedown", function () { bRight = true; SendData(); });

		var buttons = document.getElementsByClassName('button')
		for (var i = 0; i < buttons.length; i++) {
			buttons[i].addEventListener("touchend", function () {
				bForward = false;
				bBackward = false;
				bLeft = false;
				bRight = false;
				SendData();
			});
			buttons[i].addEventListener("mouseup", function () {
				bForward = false;
				bBackward = false;
				bLeft = false;
				bRight = false;
				SendData();
			});
		}

		checkboxAutodrive.addEventListener("click", function () {
			autoDrive = checkboxAutodrive.checked;
			SendData2();

			// Dissable buttons maby?
		});

		function SendData() {
			console.log('Send: /data?forward=' + (bForward ? 2 : 1) + '&backward=' + (bBackward ? 2 : 1) + '&left=' + (bLeft ? 2 : 1) + '&right=' + (bRight ? 2 : 1));
			var xhttp = new XMLHttpRequest();
			xhttp.open('GET', '/data?forward=' + (bForward ? 2 : 1) + '&backward=' + (bBackward ? 2 : 1) + '&left=' + (bLeft ? 2 : 1) + '&right=' + (bRight ? 2 : 1), true);
			xhttp.send();
		}

		function SendData2() {
			console.log('Send: /data?autodrive=' + (autoDrive ? 2 : 1));
			var xhttp = new XMLHttpRequest();
			xhttp.open('GET', '/data?autodrive=' + (autoDrive ? 2 : 1), true);
			xhttp.send();
		}
	</script>
</body>

</html>
)=====";
