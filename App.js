console.log('Program started..')

let HotwordSpotter = require('./HotwordSpotter')
let SpeechDetect = require('./SpeechDetect')

let spotter = new HotwordSpotter()
let speech = new SpeechDetect()

const colors = ['turquoise', 'rose', 'rouge', 'jaune', 'orange', 'vert']

spotter.on('hotword', async (filename) => {
    let content = await speech.getContent(filename)
    getLightColor(content)
})


function getLightColor(commandText) {
    console.log(commandText)
    let words = commandText.split(' ')

    words.forEach(word => {
        if (colors.includes(word)) {
            console.log('Changement de la couleur des lumières pour : ' + word)
        }
    });

}

