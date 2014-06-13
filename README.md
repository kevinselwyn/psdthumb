#psdthumb

Utility for extracting thumbnails from PSDs

##Description

A JPEG thumbnail is stored in every Adobe Photoshop document for quick previews and easy identification. This program finds that thumbnail and extracts it to its own file.

See the file format specification [here](http://www.adobe.com/devnet-apps/photoshop/fileformatashtml/).

##Installation

```bash
make && sudo make install
```

##Usage

```bash
psdthumb <in.psd> <out.jpg>
```
