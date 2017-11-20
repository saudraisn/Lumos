const speech = require('@google-cloud/speech');
const fs = require('fs');

process.env.GOOGLE_APPLICATION_CREDENTIALS = './speechkey.json'

// Your Google Cloud Platform project ID
const projectId = 'delta-tuner-186523';

// Creates a client
const client = new speech.SpeechClient({
    projectId: projectId,
});

class SpeechDetect {

    getContent(fileName) {

        // Reads a local audio file and converts it to base64
        const file = fs.readFileSync(fileName);
        const audioBytes = file.toString('base64');

        // The audio file's encoding, sample rate in hertz, and BCP-47 language code
        const audio = {
            content: audioBytes,
        };

        const config = {
            encoding: 'LINEAR16',
            sampleRateHertz: 16000,
            languageCode: 'fr-CA',
        };

        const request = {
            audio: audio,
            config: config,
        };

        // Detects speech in the audio file
        return client
            .recognize(request)
            .then(data => {
                const response = data[0];
                const transcription = response.results
                    .map(result => result.alternatives[0].transcript)
                    .join('\n');
                return transcription
            })
            .catch(err => {
                console.error('ERROR:', err);
            });
    }
}

module.exports = SpeechDetect