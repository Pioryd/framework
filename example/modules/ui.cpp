#include "ui.h"

#include <framework/core/application.h>
#include <framework/global.h>
#include <framework/multimedia/fontmanager.h>
#include <framework/multimedia/imagemanager.h>
#include <framework/multimedia/soundmanager.h>
#include <framework/multimedia/texturemanager.h>
#include <framework/multimedia/window.h>

namespace Modules {
void UI::init(const std::vector<std::string>& argv, const std::string& title) {
  FW::G::FontManager.init();
  FW::G::FontManager.loadFonts();
  FW::G::SoundManager.init();
  FW::G::SoundManager.loadSounds();
  FW::G::ImageManager.init();
  FW::G::ImageManager.loadImages();
  FW::G::TextureManager.init();
  FW::G::TextureManager.loadTextures();

  createUI();
}

void UI::createUI() {
  {
    auto assetsPath = FW::G::Application->getAssetsPath();

    std::filesystem::path workPath(FW::G::Application->getWorkingDirectory());

    tgui::Theme theme{assetsPath.string() + "/themes/Black.txt"};

    FW::G::Window.gui->add(tgui::Picture::create(
        assetsPath.string() + "/image/RedBackground.jpg"));

    auto tabs = tgui::Tabs::create();
    tabs->setRenderer(theme.getRenderer("Tabs"));
    tabs->setTabHeight(30);
    tabs->setPosition(70, 40);
    tabs->add("Tab - 1");
    tabs->add("Tab - 2");
    tabs->add("Tab - 3");
    FW::G::Window.gui->add(tabs);

    auto menu = tgui::MenuBar::create();
    menu->setRenderer(theme.getRenderer("MenuBar"));
    menu->setSize(static_cast<float>(FW::G::Window.window->getSize().x), 22.f);
    menu->addMenu("File");
    menu->addMenuItem("Load");
    menu->addMenuItem("Save");
    menu->addMenuItem("Exit");
    menu->addMenu("Edit");
    menu->addMenuItem("Copy");
    menu->addMenuItem("Paste");
    menu->addMenu("Help");
    menu->addMenuItem("About");
    FW::G::Window.gui->add(menu);

    auto label = tgui::Label::create();
    label->setRenderer(theme.getRenderer("Label"));
    label->setText("This is a label./nAnd these are radio buttons:");
    label->setPosition(10, 90);
    label->setTextSize(18);
    FW::G::Window.gui->add(label);

    auto radioButton = tgui::RadioButton::create();
    radioButton->setRenderer(theme.getRenderer("RadioButton"));
    radioButton->setPosition(20, 140);
    radioButton->setText("Yep!");
    radioButton->setSize(25, 25);
    FW::G::Window.gui->add(radioButton);

    radioButton = tgui::RadioButton::create();
    radioButton->setRenderer(theme.getRenderer("RadioButton"));
    radioButton->setPosition(20, 170);
    radioButton->setText("Nope!");
    radioButton->setSize(25, 25);
    FW::G::Window.gui->add(radioButton);

    radioButton = tgui::RadioButton::create();
    radioButton->setRenderer(theme.getRenderer("RadioButton"));
    radioButton->setPosition(20, 200);
    radioButton->setText("Don't know!");
    radioButton->setSize(25, 25);
    FW::G::Window.gui->add(radioButton);

    label = tgui::Label::create();
    label->setRenderer(theme.getRenderer("Label"));
    label->setText("We've got some edit boxes:");
    label->setPosition(10, 240);
    label->setTextSize(18);
    FW::G::Window.gui->add(label);

    auto editBox = tgui::EditBox::create();
    editBox->setRenderer(theme.getRenderer("EditBox"));
    editBox->setSize(200, 25);
    editBox->setTextSize(18);
    editBox->setPosition(10, 270);
    editBox->setDefaultText("Click to edit text...");
    FW::G::Window.gui->add(editBox);

    label = tgui::Label::create();
    label->setRenderer(theme.getRenderer("Label"));
    label->setText("And some list boxes too...");
    label->setPosition(10, 310);
    label->setTextSize(18);
    FW::G::Window.gui->add(label);

    auto listBox = tgui::ListBox::create();
    listBox->setRenderer(theme.getRenderer("ListBox"));
    listBox->setSize(250, 120);
    listBox->setItemHeight(24);
    listBox->setPosition(10, 340);
    listBox->addItem("Item 1");
    listBox->addItem("Item 2");
    listBox->addItem("Item 3");
    FW::G::Window.gui->add(listBox);

    label = tgui::Label::create();
    label->setRenderer(theme.getRenderer("Label"));
    label->setText("It's the progress bar below");
    label->setPosition(10, 470);
    label->setTextSize(18);
    FW::G::Window.gui->add(label);

    auto progressBar = tgui::ProgressBar::create();
    progressBar->setRenderer(theme.getRenderer("ProgressBar"));
    progressBar->setPosition(10, 500);
    progressBar->setSize(200, 20);
    progressBar->setValue(50);
    FW::G::Window.gui->add(progressBar);

    label = tgui::Label::create();
    label->setRenderer(theme.getRenderer("Label"));
    label->setText(std::to_string(progressBar->getValue()) + "%");
    label->setPosition(220, 500);
    label->setTextSize(18);
    FW::G::Window.gui->add(label);

    label = tgui::Label::create();
    label->setRenderer(theme.getRenderer("Label"));
    label->setText("That's the slider");
    label->setPosition(10, 530);
    label->setTextSize(18);
    FW::G::Window.gui->add(label);

    auto slider = tgui::Slider::create();
    slider->setRenderer(theme.getRenderer("Slider"));
    slider->setPosition(10, 560);
    slider->setSize(200, 18);
    slider->setValue(4);
    FW::G::Window.gui->add(slider);

    auto scrollbar = tgui::Scrollbar::create();
    scrollbar->setRenderer(theme.getRenderer("Scrollbar"));
    scrollbar->setPosition(380, 40);
    scrollbar->setSize(18, 540);
    scrollbar->setMaximum(100);
    scrollbar->setViewportSize(70);
    FW::G::Window.gui->add(scrollbar);

    auto comboBox = tgui::ComboBox::create();
    comboBox->setRenderer(theme.getRenderer("ComboBox"));
    comboBox->setSize(120, 21);
    comboBox->setPosition(420, 40);
    comboBox->addItem("Item 1");
    comboBox->addItem("Item 2");
    comboBox->addItem("Item 3");
    comboBox->setSelectedItem("Item 2");
    FW::G::Window.gui->add(comboBox);

    auto child = tgui::ChildWindow::create();
    child->setRenderer(theme.getRenderer("ChildWindow"));
    child->setSize(250, 120);
    child->setPosition(420, 80);
    child->setTitle("Child window");
    FW::G::Window.gui->add(child);

    label = tgui::Label::create();
    label->setRenderer(theme.getRenderer("Label"));
    label->setText("Hi! I'm a child FW::G::Window.window->");
    label->setPosition(30, 30);
    label->setTextSize(15);
    child->add(label);

    auto button = tgui::Button::create();
    button->setRenderer(theme.getRenderer("Button"));
    button->setPosition(75, 70);
    button->setText("OK");
    button->setSize(100, 30);
    button->connect("pressed", [=]() { child->setVisible(false); });
    child->add(button);

    auto checkbox = tgui::CheckBox::create();
    checkbox->setRenderer(theme.getRenderer("CheckBox"));
    checkbox->setPosition(420, 240);
    checkbox->setText("Ok, I got it");
    checkbox->setSize(25, 25);
    FW::G::Window.gui->add(checkbox);

    checkbox = tgui::CheckBox::create();
    checkbox->setRenderer(theme.getRenderer("CheckBox"));
    checkbox->setPosition(570, 240);
    checkbox->setText("No, I didn't");
    checkbox->setSize(25, 25);
    FW::G::Window.gui->add(checkbox);

    label = tgui::Label::create();
    label->setRenderer(theme.getRenderer("Label"));
    label->setText("Chatbox");
    label->setPosition(420, 280);
    label->setTextSize(18);
    FW::G::Window.gui->add(label);

    auto chatbox = tgui::ChatBox::create();
    chatbox->setRenderer(theme.getRenderer("ChatBox"));
    chatbox->setSize(300, 100);
    chatbox->setTextSize(18);
    chatbox->setPosition(420, 310);
    chatbox->setLinesStartFromTop();
    chatbox->addLine("texus: Hey, this is TGUI!", sf::Color::Green);
    chatbox->addLine("Me: Looks awesome! ;)", sf::Color::Yellow);
    chatbox->addLine("texus: Thanks! :)", sf::Color::Green);
    chatbox->addLine("Me: The widgets rock ^^", sf::Color::Yellow);
    FW::G::Window.gui->add(chatbox);

    sf::Texture texture;
    sf::Sprite sprite;
    texture.loadFromFile(assetsPath.string() + "/image/ThinkLinux.jpg");
    sprite.setTexture(texture);
    sprite.setScale(200.f / texture.getSize().x, 140.f / texture.getSize().y);

    sf::Text text{"SFML Canvas", *FW::G::Window.gui->getFont(), 24};
    text.setPosition(25, 100);
    text.setColor({200, 200, 200});

    auto canvas = tgui::Canvas::create({200, 140});
    canvas->setPosition(420, 430);
    canvas->clear();
    canvas->draw(sprite);
    canvas->draw(text);
    canvas->display();
    FW::G::Window.gui->add(canvas);

    button = tgui::Button::create();
    button->setRenderer(theme.getRenderer("Button"));
    button->setPosition(FW::G::Window.window->getSize().x - 115.f,
                        FW::G::Window.window->getSize().y - 50.f);
    button->setText("Exit");
    button->setSize(100, 40);
    button->connect("pressed", [&]() { FW::G::Window.window->close(); });
    FW::G::Window.gui->add(button);
  }
}
}  // namespace Modules
