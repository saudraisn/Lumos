console.log("UDP COMMUNICATION");
const dgram = require("dgram");

const readline = require("readline");

const rl = readline.createInterface({
  input: process.stdin,
  output: process.stdout
});
const client = dgram.createSocket("udp4");

function sendMessage(message) {
  // message = Buffer.from(message);

  client.send(message, 4210, "192.168.2.177", err => {
    // // client.close();
    // console.log("sent");
  });
}

// sendMessage("yolo man");

function colorPrompt() {
  // console.log("Entrer la couleur : ");
  rl.question("Entrer la couleur: ", answer => {
    sendMessage(answer);
    colorPrompt();
  });
}

colorPrompt();
