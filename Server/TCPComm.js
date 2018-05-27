const net = require("net");
const readline = require("readline");

const rl = readline.createInterface({
  input: process.stdin,
  output: process.stdout
});

function colorPrompt() {
  rl.question("Entrer la couleur : ", answer => {
    // TODO: Log the answer in a database
    console.log(`Thank you for your valuable feedback: ${answer}`);

    colorPrompt();
  });
}

// colorPrompt();

function sendToArduino(message) {
  var client = new net.Socket();
  client.on("data", function(data) {
    console.log("Received: " + data);
    // client.destroy(); // kill client after server's response
  });

  client.on("close", function() {
    // console.log("Connection closed");
  });

  client.connect(80, "192.168.2.177", function() {
    console.log("connected and writing : " + message);
    client.write(message);
  });
}

setInterval(() => sendToArduino("yolo"), 3000);
