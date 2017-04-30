import gifAnimation.*;

GifMaker gifExport;
BufferedReader reader;
String line;

int s = 10;

void setup() {
    size(42 * (s+5), 9 * (s+5));
    frameRate(40);

    gifExport = new GifMaker(this, "inside_out_grps.gif");
    gifExport.setRepeat(0);             // make it an "endless" animation
    gifExport.setTransparent(255, 255, 255);    // black is transparent
    
    reader = createReader("test.txt");

}

void draw() {
  background(200);
  
  for(int j=0; j<40; ++j)
  {
    try {
      line = reader.readLine();
    } catch (IOException e) {
      e.printStackTrace();
      line = null;
    }
    if (line == null) {
      // Stop reading because of an error or file is empty
      noLoop(); 
      gifExport.finish(); 
    } else {
      
      String[] t = split(line, TAB);
      String[] pieces = split(t[1], ' ');
      
      for(int i=0; i<7; ++i)
      {
        int p = int(pieces[i]);
        
        switch(p){
          case 0:
            fill(0);
            break;
          case 1:
            fill(100);
            break;
          case 2:
            fill(#B40000);
            break;
          case 3:
            fill(#FAF200);
            break;
          case 4:
            fill(#FC7F00);
            break;
        }
        rect((j+1)*(s+5), (i+1)*(s+5), s, s);
      }
      //print('\n');
    }
  }

    gifExport.setDelay(1);
    gifExport.addFrame();
}

void mousePressed() {
    gifExport.finish();                 // write file
}

