var xhrRequest = function (url, postData, callback) {
  var xhr = new XMLHttpRequest();
  xhr.onload = function () {
    callback(this.responseText);
  };
  xhr.open("POST", url);
  xhr.setRequestHeader("Content-type", "application/x-www-form-urlencoded");
  xhr.setRequestHeader("Content-length", postData.length);
  xhr.setRequestHeader("Connection", "close");
  xhr.send(postData);
};

Pebble.addEventListener('showConfiguration', function() {
  var url = 'http://midget214.github.io/simple-vrm-config/index.html';
  console.log('Showing configuration page: ' + url);

  Pebble.openURL(url);
});

var username = 'username=';
var password = 'password=';
var api_version = 'version=220';
var verification_token = 'verification_token=1';
var instance = 'instance=0';
var siteid = 'siteid=';
var api_url = 'https://juice.victronenergy.com/';
var siteCodes = 'codes=["bs", "a1", "YT", "bst", "bt", "ScS"]';
var session_id = '';

// mapping between VRM api codes and AppMessage Enums
function capitalizeFirstLetter(string) {
    return string[0].toUpperCase() + string.slice(1);
}

var dict = []; 
dict['bs'] = [ 'KEY_BS', 'valueFloat', function(value) { return value + ' %'; }];
dict['a1'] = ['KEY_A1', 'valueFloat', function(value) { return (parseFloat(value) > 1000) ? (parseFloat(value) / 1000) + " kW" :  value + ' W'; }];
dict['YT'] = ['KEY_YT', 'valueFloat', function(value) { return value + ' kWh'; }];
dict['bst'] = ['KEY_BST', 'nameEnum', function(value) { return capitalizeFirstLetter(value); }];

function appendParameter(parameter) {
  return '&' + parameter;
}

function GetSiteAttributes() {
  var url = api_url + 'sites/attributes_by_code';
  var postData = api_version + 
                 appendParameter('sessionid=' + session_id) + 
                 appendParameter(verification_token) + 
                 appendParameter(siteid) + 
                 appendParameter(instance) + 
                 appendParameter(siteCodes);
  
  xhrRequest(url, postData, function(responseText) { 
    console.log(responseText);
    var json = JSON.parse(responseText);
    
    if (json.status.code != 200)
      return;
    
    var responseDict =  {"KEY_YT":"","KEY_A1":"","KEY_BS":"","KEY_BST":""};
    var ttgo = '';
    var scs = '';
    for (var i in json.data.attributes) {
      if (json.data.attributes[i]["code"] == 'bt') {
          ttgo = json.data.attributes[i]['valueFloat'];
      }
      else if (json.data.attributes[i]["code"] == 'ScS') {
          scs = json.data.attributes[i]['nameEnum'];
      }
      else {
        var lookup = dict[json.data.attributes[i]["code"]];
      
        responseDict[lookup[0]] = lookup[2](json.data.attributes[i][lookup[1]]);
        console.log(responseDict[lookup[0]]); 
      }
    }
    
    if (responseDict['KEY_BST'] == "Discharging") {
      responseDict['KEY_BST'] += " (" + ttgo + " h)"
    }
    else if (responseDict['KEY_BST'] == "Charging") {
      responseDict['KEY_BST'] += " (" + scs + ")"
    }
    
    Pebble.sendAppMessage(responseDict,
      function(e) {
        console.log('VRM info sent to Pebble successfully!');
      },
      function(e) {
        console.log('Error sending VRM info to Pebble!');
      });
  });
}

function GetSessionID()  {
  var url = api_url + 'user/login';
  var postData = username + 
                 appendParameter(password) + 
                 appendParameter(api_version) + 
                 appendParameter(verification_token) + 
                 appendParameter(instance);
  
  
  xhrRequest(url, postData, function(responseText) { 
      console.log(responseText);
      var json = JSON.parse(responseText);
      
      if (json.status.code != 200)
        return;
    
      session_id = json.data.user.sessionid;
      console.log('Session ID : ' + session_id);
      GetSiteAttributes();
  });
}


// Listen for when the watchface is opened
Pebble.addEventListener('ready', 
  function(e) {
    console.log('PebbleKit JS ready!');
    var responseDict =  {"KEY_JS_READY":""};
    Pebble.sendAppMessage(responseDict,
      function(e) {
        console.log('Ready notfication sent!');
      },
      function(e) {
        console.log('Error sending Ready notfication!');
      });
  }
);

// Listen for when an AppMessage is received
Pebble.addEventListener('appmessage',
  function(e) {
    console.log('AppMessage received!');
    GetSessionID();
  }                     
);