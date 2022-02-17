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
    tableComponent.getHeader().addColumn("Track Name", 1, 590);
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
    //setColour(juce::ListBox::backgroundColourId, juce::Colours::darkblue);
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
        g.fillAll(juce::Colours::lightblue);
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
    g.setFont(juce::Font{(float)height, juce::Font::bold});
    //g.setColour(juce::Colours::red);
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
            juce::File file = trackNameAndFile.at(trackTitles[rowNumber]);
            g.drawText(file.getFileExtension(),
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
    if(existingComponentToUpdate == nullptr){
        if(columnId == 3){
            std::string buttonType = "load";
            existingComponentToUpdate = createNewButton(buttonType, rowNumber);
        }else if(columnId == 4){
            std::string buttonType = "remove";
            existingComponentToUpdate = createNewButton(buttonType, rowNumber);
        }
    }
    return existingComponentToUpdate;
}



void PlaylistComponent::buttonClicked(juce::Button* button) {
    std::string buttonText = button->getButtonText().toStdString();
    int id = std::stoi(button->getComponentID().toStdString());
    if(buttonText == "load"){
        deckGUIGroup.loadURLintoOnePlayer(trackNameAndFile.at(trackTitles[id]));
    }
    else if(buttonText == "remove"){
        allTrackFiles.erase(trackNameAndFile[trackTitles[id]].getFullPathName());
        allTrackTitles[trackTitles[id]] = false;
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
            return true;
        }
    }
}
void PlaylistComponent::filesDropped (const juce::StringArray &files, int x, int y){
    for(auto& fileName: files){
        //iterates through all dropped files and stores its information in different
        //data structures used to display the playlist cell
            convertLineToFileEntry(fileName.toStdString());
    }
    //after storing all information of the dropped files, refresh the playlist
    updatePlayList();
}

void PlaylistComponent::savePlaylist()
{
    std::ofstream outFile(playlistFilePath);
    for (const auto &eachFile : allTrackFiles){
        outFile << eachFile.getFullPathName() << "\n";
    }
}

void PlaylistComponent::loadPlaylist() {
    std::ifstream file{playlistFilePath};
    std::string line;
    while(std::getline(file, line)){
        convertLineToFileEntry(line);
    }
    updatePlayList();
}

void PlaylistComponent::convertLineToFileEntry(std::string filePath) {
    juce::File eachFile{filePath};
    //std::cout<<"file is "<< eachFile.getFullPathName()<<std::endl;
    if(allTrackFiles.count(eachFile) == 0){
        trackTitles.push_back(eachFile.getFileName().toStdString());
        allTrackTitles.insert({eachFile.getFileName().toStdString(), true});
        allTrackFiles.insert(eachFile);
        trackNameAndFile.insert({eachFile.getFileName().toStdString(), eachFile});
    }
}

void PlaylistComponent::updatePlayList() {
    tableComponent.updateContent();
    tableComponent.repaint();
}

void PlaylistComponent::textEditorTextChanged(juce::TextEditor & textEditor) {
    std::string searchResult = textEditor.getText().toStdString();
    std::cout<<searchResult<<std::endl;
    if(!isSearchFieldEmpty()){
        putSearchResultIntoPlaylist(searchResult);
    }else{
        for(auto& eachTrackTitle: allTrackTitles){
            for(int i = 0; i < trackTitles.size(); i++){
                if(std::find(trackTitles.begin(), trackTitles.end(), eachTrackTitle.first)==trackTitles.end() && eachTrackTitle.second == true){
                    trackTitles.push_back(eachTrackTitle.first);
                    allTrackFiles.insert(trackNameAndFile[eachTrackTitle.first]);
                }
            }
        }
        updatePlayList();
    }
}

bool PlaylistComponent::isSearchFieldEmpty() {
    return playlistSearchField.isEmpty();
}

void PlaylistComponent::putSearchResultIntoPlaylist(std::string &result) {
    for(int i = 0; i < trackTitles.size(); i++){
        if(trackTitles[i].find(result) == std::string::npos){
            juce::File& file = trackNameAndFile[trackTitles[i]];
            allTrackFiles.erase(trackNameAndFile[trackTitles[i]]);
            trackTitles.erase(trackTitles.begin() + i);
        }
    }
    updatePlayList();

}

juce::TextButton *PlaylistComponent::createNewButton(std::string & name, int rowNumber) {
    juce::TextButton* btn = new juce::TextButton{name};
    juce::String id{std::to_string(rowNumber)};
    btn->setComponentID(id);
    btn->addListener(this);
    return btn;
}

