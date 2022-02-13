/*
  ==============================================================================

    PlaylistComponent.cpp
    Created: 9 Feb 2022 11:03:20pm
    Author:  matt

  ==============================================================================
*/
#include <JuceHeader.h>
#include "PlaylistComponent.h"

//==============================================================================
PlaylistComponent::PlaylistComponent(juce::AudioFormatManager &_formatManager, DeckGUIGroup& _deckGUIGroup):
formatManager(_formatManager),
deckGUIGroup(_deckGUIGroup),
playlistFilePath("./play_list_file.txt")

{
    tableComponent.getHeader().addColumn("Track Name", 1, 400);
    tableComponent.getHeader().addColumn("Track Type", 2, 100);
    tableComponent.getHeader().addColumn("", 3, 100);
    tableComponent.getHeader().addColumn("", 4, 100);

    tableComponent.setModel(this);

    addAndMakeVisible(tableComponent);
    addAndMakeVisible(playlistSearchField);
    playlistSearchField.setTextToShowWhenEmpty("Search for a song...", juce::Colours::white);
    playlistSearchField.addListener(this);
    loadPlaylist();
}

PlaylistComponent::~PlaylistComponent()
{
    savePlaylist();
}

void PlaylistComponent::paint (juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (juce::Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (juce::Colours::white);
    g.setFont (14.0f);
    g.drawText ("PlaylistComponent", getLocalBounds(),
                juce::Justification::centred, true);   // draw some placeholder text
}

void PlaylistComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
    tableComponent.setBounds(0,0,getWidth(), getHeight()*0.9);
    playlistSearchField.setBounds(0,getHeight()*0.9,getWidth()/6,getHeight()*0.1 );
}

int PlaylistComponent::getNumRows() {
    return trackTitles.size();
}

void PlaylistComponent::paintRowBackground(
        juce::Graphics &g,
        int rowNumber,
        int width,
        int height,
        bool rowIsSelected
)
{
    if(rowIsSelected){
        g.fillAll(juce::Colours::orange);
    }else{
        g.fillAll(juce::Colours::darkgrey);
    }
}

void PlaylistComponent::paintCell(
        juce::Graphics &g,
        int rowNumber,
        int columnId,
        int width,
        int height,
        bool rowIsSelected
)
{

    switch (columnId) {
        case 1:{
            g.drawText(trackTitles[rowNumber],
                       2,
                       0,
                       width-4,
                       height,
                       juce::Justification::centredLeft, true
            );
            break;
        }
        case 2:{
            juce::File fileInfo = fileStatus.at(trackTitles[rowNumber]);
            g.drawText(fileInfo.getFileExtension(),
                       2,
                       0,
                       width-4,
                       height,
                       juce::Justification::centredLeft, true
            );
            break;
        }
        default:{
            break;
        }
    };

}

juce::Component *PlaylistComponent::refreshComponentForCell(
        int rowNumber,
        int columnId,
        bool isRowSelected,
        juce::Component *existingComponentToUpdate
)
{
    juce::TextButton* btn;
    if(columnId == 3){
        if(existingComponentToUpdate == nullptr){
            btn = new juce::TextButton{"play"};
            juce::String id{std::to_string(rowNumber)};
            btn->setComponentID(id);
            btn->addListener(this);
            existingComponentToUpdate = btn;
        }
    }else if(columnId == 4){
        if(existingComponentToUpdate == nullptr){
            btn = new juce::TextButton{"remove"};
            juce::String id{std::to_string(rowNumber)};
            btn->setComponentID(id);
            btn->addListener(this);
            existingComponentToUpdate = btn;
        }
    }


    return existingComponentToUpdate;
}



void PlaylistComponent::buttonClicked(juce::Button* button) {
    std::string buttonText = button->getButtonText().toStdString();
    int id = std::stoi(button->getComponentID().toStdString());
    if(buttonText == "play"){
        std::cout<<"button clicked "<< trackTitles[id] <<std::endl;
        deckGUIGroup.loadURLintoOnePlayer(fileStatus.at(trackTitles[id]));
    }
    else if(buttonText == "remove"){
        std::cout<<"track path: "<<trackTitles[id]<<std::endl;
        allTracks.erase(fileStatus[trackTitles[id]].getFullPathName());
        fileStatus.erase(trackTitles[id]);
        trackTitles.erase(trackTitles.begin()+id);
        updatePlayList();
    }
}



bool PlaylistComponent::isInterestedInFileDrag(const juce::StringArray &files) {
    for(auto& fileName: files){
        juce::File eachFile{fileName};
        std::unique_ptr<juce::AudioFormatReader> reader{formatManager.createReaderFor(eachFile)};
        if(reader == nullptr){
            return false;
        }else{
            juce::URL fileRead{eachFile};
            std::cout<<"file name "<< eachFile.getFileName()<<std::endl;
            std::cout<<"file type "<< eachFile.getFileExtension()<<std::endl;
            return true;
        }
    }
}
void PlaylistComponent::filesDropped (const juce::StringArray &files, int x, int y){
    for(auto& fileName: files){
            //juce::File eachFile{fileName};
            //std::cout<<"file is "<< eachFile.getFullPathName()<<std::endl;
            convertLineToFileEntry(fileName.toStdString());
    }
    updatePlayList();
}

void PlaylistComponent::savePlaylist()
{
    std::ofstream outFile(playlistFilePath);
    for (const auto &eachFile : allTracks){
        outFile << eachFile.getFullPathName() << "\n";
    }
    // the important part
}

void PlaylistComponent::loadPlaylist() {
    std::ifstream file{playlistFilePath};
    std::string line;
    while(std::getline(file, line)){
        convertLineToFileEntry(line);
    }
    updatePlayList();
}

void PlaylistComponent::convertLineToFileEntry(std::string line) {
    juce::File eachFile{line};
    //std::cout<<"file is "<< eachFile.getFullPathName()<<std::endl;
    if(allTracks.count(eachFile) == 0){
        trackTitles.push_back(eachFile.getFileName().toStdString());
        allTracks.insert(eachFile);
        fileStatus.insert({eachFile.getFileName().toStdString(), eachFile});
    }
}

void PlaylistComponent::updatePlayList() {
    tableComponent.updateContent();
    tableComponent.repaint();
}

void PlaylistComponent::textEditorTextChanged(juce::TextEditor & textEditor) {
    std::string searchResult = textEditor.getText().toStdString();
    std::cout<<searchResult<<std::endl;
    if(isSearchFieldEmpty()){
        std::cout<<"empty"<<std::endl;
    }
    putSearchResultIntoPlaylist(searchResult);
}

bool PlaylistComponent::isSearchFieldEmpty() {
    return playlistSearchField.isEmpty();
}

void PlaylistComponent::putSearchResultIntoPlaylist(std::string &result) {
    std::vector<std::string> searchResult;
    for(std::string& trackName:trackTitles){
        if(trackName.find(result) != std::string::npos){
            searchResult.push_back(trackName);
        }
    }
}

