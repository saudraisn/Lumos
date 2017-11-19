let Mic = require('node-microphone')
let Stream = require('stream')
let mic = new Mic()
let micStream = mic.startRecording()

// let stream = new Stream()
// micStream.pipe(myWritableStream)

setTimeout(() => {
  console.log('stopped recording')
  mic.stopRecording()
}, 3000)

mic.on('info', info => {
  console.log(info)
})

mic.on('error', error => {
  console.log(error)
})
