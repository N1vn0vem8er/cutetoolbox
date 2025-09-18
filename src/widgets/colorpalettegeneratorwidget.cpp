#include "colorpalettegeneratorwidget.h"
#include "src/widgets/ui_colorpalettegeneratorwidget.h"

ColorPaletteGeneratorWidget::ColorPaletteGeneratorWidget(QWidget *parent)
    : CustomWidget(parent)
    , ui(new Ui::ColorPaletteGeneratorWidget)
{
    ui->setupUi(this);
    setName(tr("Color Palette Generator"));
}

ColorPaletteGeneratorWidget::~ColorPaletteGeneratorWidget()
{
    delete ui;
}
