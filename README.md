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
# how to use
You can use it in any macro in principle you should call '  new Bober::HistogramEditor(h, gPad);' to start editor. 'h' is your histogram (TH1 and TH2 are supported) and gPad is pointer to pad for this histogram.
When you finish your setting up the histogram you can go to tab 'Super' and set the path to the xml-file with configuration. You can configure only one pad+histogram per single xml file.
To apply settings you have to import them e.g.:
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
