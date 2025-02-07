Gltch quando rimangon accesi i pixel dalla immagine prima

fix
- tutto nero (potrebbe fare effetto blink)
- altrimenti diffing come mpeg


un po' simile a immediate vs retained UI, anche se React fa il diffing quindi è una via di mezzo (fa il diffing dell'intera App ogni volta, ma non la ridisegna se non c'è bisogno)
