# Bober
# Because Of Basic Editor in Root sucks
GUI editor for root histogram.
As you know it's annoying to configure the histograms. Here is a GUI editor that will help you.
## How to install
1. goto repo directory
2. make build directory 'mkdir build'
3. go there 'cd build'
4. call 'cmake ..'
5. compile code 'make -jN' where N is number of cores
6. to use Bober apped build directory to library path (e.g., 'export LD_LIBRARY_PATH=~/bober/build:$LD_LIBRARY_PATH')
## Usage Guide

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
