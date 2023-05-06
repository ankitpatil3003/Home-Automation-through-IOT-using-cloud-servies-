var firebaseConfig = {
	apiKey: "AIzaSyAfDLzhRaoOXkq7w_RTolBMkBkfm7j_Dhs",
	authDomain: "cloud-toggle-switch.firebaseapp.com",
	projectId: "cloud-toggle-switch",
	storageBucket: "cloud-toggle-switch.appspot.com",
	messagingSenderId: "703071408902",
	appId: "1:703071408902:web:937f1d8b1e2cc339a30487",
	measurementId: "G-K4Y8S26ZFT"
  };
  // Initialize Firebase
  firebase.initializeApp(firebaseConfig);
 
  
  $(document).ready(function(){
	var database = firebase.database();
	var Led1Status;
  
	database.ref().on("value", function(snap){
	  if(Led1Status == "1"){
		document.getElementById("unact").style.display = "none";
		document.getElementById("act").style.display = "block";
	  } else {
		document.getElementById("unact").style.display = "block";
		document.getElementById("act").style.display = "none";
	  }
	});
  
	// Speech Recognition
	var recognition = new webkitSpeechRecognition();
	recognition.continuous = true;
	recognition.interimResults = true;
	recognition.lang = "en-US";
	var isListening = false;
  
	$("#start-btn").click(function(){
		if (!isListening) {
		  recognition.start();
		  isListening = true;
		  $(this).addClass('active');
		  $(this).text("listening...");
		} else {
			
				recognition.stop();
				isListening = false;
				$(this).removeClass('active');
				$(this).text("Start Listening");
			  
		}
	  });
  
	  recognition.onend = function() {
		isListening = false;
		$("#start-btn").removeClass('active');
	  };

	recognition.onresult = function(event) {
		var result = event.results[event.results.length - 1];
		var text = result[0].transcript.toLowerCase().trim();
		console.log(text);
	  
		if(text.includes("turn on") || text.includes("on the light") || text.includes("light on")){
		  var firebaseRef = firebase.database().ref().child("Led1Status/status");
		  firebaseRef.set("on");
		  Led1Status = "0";
		  users1="you have just turned on the light";
		  document.getElementById("/demo1").innerHTML = users1;
		  database.ref("Led1Status/les").on("value", function(snapshot) {
			var users = snapshot.val();
			console.log(users);
			document.getElementById("/demo2").innerHTML = users;
		});
		  
		}
		else if(text.includes("turn off") || text.includes("off the light") || text.includes("light off")){
		  var firebaseRef = firebase.database().ref().child("Led1Status/status");
		  firebaseRef.set("off");
		  Led1Status = "1";
		  users="you have just turned off the light";
		  document.getElementById("/demo1").innerHTML = users;
		}
	  };
	  
  
	// Toggle Button
	$(".toggle-btn").click(function(){
	  var firebaseRef = firebase.database().ref().child("Led1Status/status");
	  if(Led1Status == "1") {
		firebaseRef.set("on");
		Led1Status = "0";
		database.ref("Led1Status/les").on("value", function(snapshot) {
			var users = snapshot.val();
			console.log(users);
			document.getElementById("/demo2").innerHTML = users;
		});
		users1 = "You have just turned on the lights";
		document.getElementById("/demo1").innerHTML = users1;
	} else {
		firebaseRef.set("off");
		Led1Status = "1";
		users = "You have just turned off the lights";
		document.getElementById("/demo1").innerHTML = users;
	}
	});
  });