```

██████   ██████  ██████  ███████ ██████  
██   ██ ██    ██ ██   ██ ██      ██   ██ 
██████  ██    ██ ██████  █████   ██████  
██   ██ ██    ██ ██   ██ ██      ██   ██ 
██████   ██████  ██████  ███████ ██   ██ 

```
  
# Because Of Basic Editor in Root sucks
GUI editor for root histogram.
As you know it's annoying to configure the histograms. Here is a GUI editor that will help you.
## How to install
1. goto repo directory (Bober)
2. make build directory 'mkdir build'
3. go there 'cd build'
4. call 'cmake .. -DCMAKE_INSTALL_PREFIX=<path to install>'
5. compile code 'make -jN' where N is number of cores
6. call 'make install'
7. you are done :)

## Usage Guide

### Setting up enviroment
To allow Bober works correclty you have to set environmental path, in principle by command:
```
export LD_LIBRARY_PATH=<path_to_boberinstall>/lib:$LD_LIBRARY_PATH
```
### Starting the Editor
To launch the histogram editor, create a new instance of `Bober::HistogramEditor`:

```cpp
// h: pointer to your histogram (TH1 or TH2)
// gPad: pointer to the current pad
auto editor = new Bober::HistogramEditor(h, gPad);
```

### Saving Configuration
1. Configure your histogram using the editor
2. Go to the 'Super' tab
3. Set the path to save your XML configuration

Note: Each XML file can store settings for one pad-histogram combination.

### Applying Saved Configuration
To apply previously saved settings:

```
  Bober::HistoStyle style;
  Bober::XMLFile file("t.xml");
  auto root = file.GetRootNode();
  style.ImportFromXML(root->GetChild("histogram"));
  style.Apply(*h);
  Bober::PadStyle padStyle;
  padStyle.ImportFromXML(root->GetChild("pad"));
  padStyle.Apply(gPad);
```

### Demo
There are demo files prepared.
After installation in <install_path>/bin you have demo.exe file. This is application that generates random TH1 histogram. Note: do not forget 
to setup LD_LIBRARY_PATH before!
To open created file you can use tester.C macro (this is stand alone root macro, it's not compilec and installed in bin, sou
you should modify path to opened xml file.

### Bugs
This is a early developed project, if you found bug please report as an issue to github.
