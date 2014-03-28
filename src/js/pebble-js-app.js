function findPebbleAndSendMessage() {
  
  if(navigator.geolocation) {
    console.log("Navigator geolocation on!");

    var timeoutVal = 10 * 1000 * 1000;
    navigator.geolocation.getCurrentPosition(
       getPosition, 
       displayError,
     { enableHighAccuracy: true, timeout: timeoutVal, maximumAge: 0 }
    );
  }
  
}

function getPosition(position) {
  var latitude = position.coords.latitude;
  var longitude = position.coords.longitude;
  
  sendMessage(latitude, longitude);
  
  
}

function displayError(error) {
  
}

function sendMessage(latitude, longitude) {
  var response;
  var msg = {};
  var req = new XMLHttpRequest();
  
  req.open('GET', 'http://illinidumps.com/api/locations/close?latitude='+latitude+'&longitude='+longitude, true);
  
  req.onload = function(e) {
    if(req.readyState == 4) {
      if(req.status == 200) {
        console.log(req.responseText);
        response = JSON.parse(req.responseText);
        msg.init = true;
        msg.name = response.name;
        msg.score = "Score: " + response.score;
        msg.smell = "Smell: " + response.smell_score;
        msg.crowd = "Crowd: " + response.crowd_score;
        msg.clean = "Clean: " + response.clean_score;
        
        console.log(msg);
        Pebble.sendAppMessage(msg);
        console.log("Message sent");
        
      } else {
        console.log("Request returned error code " + req.status.toString());
      }
    } else {
      console.log("Request in state " + req.readyState);
    }
  };
  
  req.send(null);
}


Pebble.addEventListener("ready",
  function(e) {
    console.log("JavaScript app ready and running!");
    findPebbleAndSendMessage();

  }
);