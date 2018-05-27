const record = require('node-record-lpcm16')
const snowboy = require('./lib/snowboy/lib/node/index')
const fs = require('fs')
const CircularBuffer = require('circular-buffer')
const header = require('waveheader')
const { EventEmitter } = require('events')
const Detector = snowboy.Detector
const Models = snowboy.Models

const recordOptions = {
  threshold: 0,
  verbose: false,
}

class HotwordSpotter extends EventEmitter {
  constructor() {
    super()
    this.buildModels()
    this.initDetector()
    this.initMic()
    // this.initStreams()
  }

  buildModels() {
    this.models = new Models()

    this.models.add({
      file: 'Lumos.pmdl',
      sensitivity: '0.5',
      hotwords: 'Lumos',
    })

    this.models.add({
      file: 'Party.pmdl',
      sensitivity: '0.5',
      hotwords: 'Party',
    })

    this.models.add({
      file: 'Blue.pmdl',
      sensitivity: '0.5',
      hotwords: 'Blue',
    })

    this.models.add({
      file: 'Sexy.pmdl',
      sensitivity: '0.5',
      hotwords: 'Sexy',
    })

    this.models.add({
      file: 'Rainbow.pmdl',
      sensitivity: '0.5',
      hotwords: 'Rainbow',
    })
  }

  initDetector() {
    this.detector = new Detector({
      resource: 'common.res',
      models: this.models,
      audioGain: 1.0,
    })
    this.detector.on('silence', () => {})
    this.detector.on('sound', buffer => {
      // console.log('A')
    })
    this.detector.on('error', err => {
      console.log('error', err)
    })

    this.detector.on('hotword', (index, hotword, buffer) => {
      this.hotwordDetected = true
      // this.saveLumosCommand()
      // console.log(`${hotword}!!!!!!!!!!!!`)
      this.emit('hotword', hotword)
    })
  }

  initMic() {
    this.mic = record.start(recordOptions)
    this.mic.pipe(this.detector)
  }

  initStreams() {
    this.mics = [{}, {}, {}]
    this.streams = [{}, {}, {}]
    this.streamIndex = 0
    this.hotwordDetected = false

    setInterval(() => {
      if (!this.hotwordDetected) {
        if (this.streams[this.streamIndex].close) {
          this.streams[this.streamIndex].close()
        }

        this.mics[this.streamIndex] = record.start(recordOptions)
        this.streams[this.streamIndex] = fs.createWriteStream(`test${this.streamIndex}.wav`, {
          encoding: 'binary',
        })
        this.mics[this.streamIndex].pipe(this.streams[this.streamIndex])

        this.streamIndex++
        this.streamIndex = this.streamIndex % this.streams.length
      }
    }, 1000)
  }

  saveLumosCommand() {
    return new Promise((resolve, reject) => {
      setTimeout(() => {
        this.streams.forEach(stream => {
          if (stream.close) {
            stream.close()
          }
        })

        this.emit('hotword', `./test${this.streamIndex}.wav`)
      }, 3000)
    })
  }
}

module.exports = HotwordSpotter
