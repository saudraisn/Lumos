console.log("Program started..");
require("./UDPComm");

// const SerialPort = require('serialport')
// const Readline = SerialPort.parsers.Readline
// const port = new SerialPort('/dev/ttyUSB0')

// port.on('data', data => {
//   console.log('Arduino output : ' + data.toString())
// })

// let HotwordSpotter = require('./HotwordSpotter')
// // let SpeechDetect = require('./SpeechDetect')

// let spotter = new HotwordSpotter()
// // let speech = new SpeechDetect()

// const colors = ['turquoise', 'rose', 'rouge', 'jaune', 'orange', 'vert']

// spotter.on('hotword', hotword => {
//   console.log(hotword + ' detected.')
//   port.write(hotword.charAt(0))
// })

////////////////////////////////////////////////////////////////////////

// spotter.on('hotword', async filename => {
//   let content = await speech.getContent(filename)
//   getLightColor(content)
// })

// function getLightColor(commandText) {
//   if (commandText) {
//     console.log(commandText)
//     let words = commandText.split(' ')

//     words.forEach(word => {
//       if (colors.includes(word)) {
//         console.log('Changement de la couleur des lumières pour : ' + word)
//       }
//     })
//   } else {
//     console.log('No command found.')
//   }
// }
