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
  console.log("something went wrong with the geolocator");
  console.log(error);
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

        if(!response) {
          msg.name = "Can't find bathroom";
          msg.score = " "; // hack to clear "GPS fix" msg
        } else {
          var name, score, votes, smell_score, crowd_score, clean_score;

          name = response.name;
          score = response.score;
          votes = response.votes;
          smell_score = response.smell_score;
          crowd_score = response.crowd_score;
          clean_score = response.clean_score;
          
          if(!score) score = "-";
          if(!smell_score) smell_score = "x.x";
          if(!crowd_score) crowd_score = "x.x";
          if(!clean_score) clean_score = "x.x";
          
          msg.name = name;
          msg.score = "Score: " + score + " on " + votes + " votes";
          msg.smell = "Smelliness: " + smell_score;
          msg.crowd = "Crowdedness: " + crowd_score;
          msg.clean = "Cleanliness: " + clean_score;          
        }
        
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
    console.log("Javascript app ready and running!");
    findPebbleAndSendMessage();

  }
);