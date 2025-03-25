#include "metrics.h"
#include "lexer.h"
#include <iostream>
#include <iomanip>
#include <QtWidgets/QApplication>
#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtCharts/QLegendMarker>
#include <QtCharts/QLineSeries>
#include <QToolTip>
#include <QPdfWriter>
#include <QPainter>
#include <QFileDialog>
#include <QDebug>

QT_CHARTS_USE_NAMESPACE

void visualizemetrics(const std::vector<FileMetrics> &allMetrics)
{
    int argc = 0;
    char *argv[] = {nullptr};
    QApplication app(argc, argv);

    QMainWindow window;
    QWidget *centralWidget = new QWidget(&window);
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);

    QChartView *chartView = new QChartView();
    QChart *chart = new QChart();
    chart->setTitle("Code Size Metrics");

    QBarSeries *sizeSeries = new QBarSeries();
    QBarSeries *ccSeries = new QBarSeries();
    ccSeries->setName("Cyclomatic Complexity");
    QBarSeries *Tokens = new QBarSeries();
    Tokens->setName("Additional Metrics");

    std::vector<QBarSet *> tokenBars;
    QStringList tokenNames = {"Operands", "Operators", "Unique Operands", "Unique Operators","Comment Density"};
    for (const auto &metric : tokenNames)
    {
        tokenBars.push_back(new QBarSet(metric));
    }

    QBarSeries *halsteadSeries = new QBarSeries();
    std::vector<QBarSet *> barSetsHalstead;
    QStringList halsteadSets = {"Vocabulary", "Length", "Calculated length", "Volume", "Difficulty", "Effort", "Time", "Bugs"};
    for (const auto &metric : halsteadSets)
    {
        barSetsHalstead.push_back(new QBarSet(metric));
    }
    std::vector<QBarSet *> barSets;
    QStringList categories;
    QString metricNames[] = {"TotalLines", "CodeLines", "BlankLines", "Functions", "TotalComments", "MultiLineComments"};

    for (int i = 0; i < 6; ++i)
    {
        barSets.push_back(new QBarSet(metricNames[i]));
    }
    std::vector<QBarSet *> ccSets;
    QStringList categories1;
    QString metricNames1[] = {"CyclomaticComplexity", "CyclomaticDensity"};
    for (int i = 0; i < 2; ++i)
    {
        ccSets.push_back(new QBarSet(metricNames1[i]));
    }
    for (const auto &fileMetrics : allMetrics)
    {
        categories << QString::fromStdString(fileMetrics.filename);
        *barSets[0] << fileMetrics.metrics.totalLines;
        *barSets[1] << fileMetrics.metrics.totalLines - fileMetrics.metrics.blankLines - fileMetrics.metrics1.totalCommentLines;
        *barSets[2] << fileMetrics.metrics.blankLines;
        *barSets[3] << fileMetrics.metrics.functionCount;
        *barSets[4] << fileMetrics.metrics1.totalCommentLines;
        *barSets[5] << fileMetrics.metrics1.multiLineComments;
        //*barSets[6] << (fileMetrics.metrics1.totalCommentLines > 0 ? (fileMetrics.metrics1.totalCommentLines / (double)fileMetrics.metrics.totalLines) * 100 : 0);

        int codeLines = fileMetrics.metrics.totalLines - fileMetrics.metrics.blankLines - fileMetrics.metrics1.totalCommentLines;
        double density = (codeLines > 0) ? ((double)(fileMetrics.metrics3.cyclomaticComplexity) / codeLines) * 100 : 0.00;

        *ccSets[0] << fileMetrics.metrics3.cyclomaticComplexity;
        *ccSets[1] << density;
        *barSetsHalstead[0] << fileMetrics.metrics4.vocabulary;
        *barSetsHalstead[1] << fileMetrics.metrics4.length;
        *barSetsHalstead[2] << fileMetrics.metrics4.calculatedLength;
        *barSetsHalstead[3] << fileMetrics.metrics4.volume;
        *barSetsHalstead[4] << fileMetrics.metrics4.difficulty;
        *barSetsHalstead[5] << fileMetrics.metrics4.effort;
        *barSetsHalstead[6] << fileMetrics.metrics4.time;
        *barSetsHalstead[7] << fileMetrics.metrics4.bugs;
        *tokenBars[0] << fileMetrics.metrics4.N2;
        *tokenBars[1] << fileMetrics.metrics4.N1;
        *tokenBars[2] << fileMetrics.metrics4.n2;
        *tokenBars[3] << fileMetrics.metrics4.n1;
        *tokenBars[4] << (fileMetrics.metrics1.totalCommentLines > 0 ? (fileMetrics.metrics1.totalCommentLines / (double)fileMetrics.metrics.totalLines) * 100 : 0);
    }

    for (auto *barSet : barSets)
    {
        sizeSeries->append(barSet);
    }
    for (auto *barSet : barSetsHalstead)
    {
        halsteadSeries->append(barSet);
    }
    for (auto *barSet : ccSets)
    {
        ccSeries->append(barSet);
    }
    for (auto *barSet : tokenBars)
    {
        Tokens->append(barSet);
    }
    chart->addSeries(sizeSeries);

    QBarCategoryAxis *xAxis = new QBarCategoryAxis();
    xAxis->append(categories);
    xAxis->setTitleText("Files");
    chart->addAxis(xAxis, Qt::AlignBottom);
    sizeSeries->attachAxis(xAxis);
    ccSeries->attachAxis(xAxis);
    Tokens->attachAxis(xAxis);

    QValueAxis *yAxis = new QValueAxis();
    int maxTotalLines = 0;
    for (const auto &fileMetrics : allMetrics)
    {
        if (fileMetrics.metrics.totalLines > maxTotalLines)
        {
            maxTotalLines = fileMetrics.metrics.totalLines;
        }
    }
    yAxis->setTitleText("Metric Values");
    yAxis->setRange(0, maxTotalLines + 10);
    yAxis->setTickCount(10);
    yAxis->setLabelFormat("%d");

    chart->addAxis(yAxis, Qt::AlignLeft);

    sizeSeries->attachAxis(yAxis);
    ccSeries->attachAxis(yAxis);
    Tokens->attachAxis(yAxis);
    chartView->setChart(chart);

    QPushButton *toggleButton = new QPushButton("Change Mode");
    QPushButton *exportButton = new QPushButton("Save as PDF");
    QPushButton *switchChartButton = new QPushButton("Show Cyclomatic Complexity");
    QPushButton *showHalsteadButton = new QPushButton("Show Halstead Metrics");
    QPushButton *showTokens = new QPushButton("Show Additional Metrics");
    QPushButton *exitButton = new QPushButton("Exit");

    bool showingCyclomatic = false;
    bool showingHalstead = false;
    bool showingTokens = false;
    QObject::connect(switchChartButton, &QPushButton::clicked, [&]()
                     {
                         if (chart->series().contains(ccSeries)) {
                            chart->removeSeries(ccSeries);
                        }
                        if (chart->series().contains(sizeSeries)) {
                            chart->removeSeries(sizeSeries);
                        }
                        if(chart->series().contains(halsteadSeries)){
                            chart->removeSeries(halsteadSeries);
                        }
                        showingCyclomatic = !showingCyclomatic;
                        showingHalstead = false;
                        if (showingCyclomatic) {
                            chart->setTitle("Cyclomatic Complexity Metrics");
                            chart->addSeries(ccSeries);

                            yAxis->setRange(0, 50);
                            ccSeries->attachAxis(xAxis);
                            ccSeries->attachAxis(yAxis);
                
                            switchChartButton->setText("Show Code Size Metrics");
                            showHalsteadButton->setVisible(true);
                            switchChartButton->setVisible(true);
                        }
                        else {
                            chart->setTitle("Code Size Metrics");
                            chart->addSeries(sizeSeries);
                            sizeSeries->attachAxis(xAxis);
                            sizeSeries->attachAxis(yAxis);
                            int maxTotalLines = 0;
                            for (const auto &fileMetrics : allMetrics)
                            {
                                if (fileMetrics.metrics.totalLines > maxTotalLines)
                                    {
                                        maxTotalLines = fileMetrics.metrics.totalLines;
                                    }
                            }
                            yAxis->setRange(0,maxTotalLines+10);   
                            switchChartButton->setText("Show Cyclomatic Complexity");
                
                            showHalsteadButton->setVisible(true);
                            switchChartButton->setVisible(true);int maxeffort = 0,maxvolume = 0;
                        } });

    QObject::connect(showHalsteadButton, &QPushButton::clicked, [&]()
                     {
        chart->setTitle("Halstead Metrics");
        chart->removeAllSeries();
        chart->addSeries(halsteadSeries);
        int maxeffort = 0,maxvolume = 0;
    for(const auto &FileMetrics : allMetrics){
        if(FileMetrics.metrics4.effort > maxeffort){
            maxeffort = FileMetrics.metrics4.effort;
    }
        if(FileMetrics.metrics4.volume > maxvolume){
            maxvolume = FileMetrics.metrics4.volume;
        }
        int maxvalue = std::max(maxeffort,maxvolume);
        yAxis->setRange(0, maxvalue+10);
        halsteadSeries->attachAxis(xAxis);
        halsteadSeries->attachAxis(yAxis);
        switchChartButton->setVisible(false);
        showHalsteadButton->setVisible(false);
    } });
    QObject::connect(showTokens, &QPushButton::clicked, [&]()
                     {
        chart->setTitle("Additional Metrics");
        chart->removeAllSeries();
        chart->addSeries(Tokens);
        int maxValue = 0;
        for(const auto &FileMetrics : allMetrics){
            if(FileMetrics.metrics4.N2 > maxValue){
                maxValue = FileMetrics.metrics4.N2;
            }
        }
        yAxis->setRange(0, maxValue+10); 
        Tokens->attachAxis(xAxis);
        Tokens->attachAxis(yAxis);
        switchChartButton->setVisible(false);
        showHalsteadButton->setVisible(false);
        showTokens->setVisible(false); });

    bool darkMode = false;

    QObject::connect(exitButton, &QPushButton::clicked, [&]()
                     { QApplication::quit(); });

    QObject::connect(exportButton, &QPushButton::clicked, [&]()
                     {
        QString fileName = QFileDialog::getSaveFileName(nullptr, "Save Chart as PDF", "", "PDF Files (*.pdf)");
        if (fileName.isEmpty())
            return;
        QPdfWriter pdfWriter(fileName);
        pdfWriter.setPageSize(QPageSize(QPageSize::A4));
        QPainter painter(&pdfWriter);
        chartView->render(&painter);
        painter.end(); });

    QObject::connect(toggleButton, &QPushButton::clicked, [&]()
                     {
        darkMode = !darkMode;
        if (darkMode)
        {
            chart->setTheme(QChart::ChartThemeDark);
            qApp->setStyleSheet("background-color: #2E2E2E; color: white;");
            toggleButton->setStyleSheet("background-color: #555; color: white;");
        }
        else
        {
            chart->setTheme(QChart::ChartThemeLight);
            qApp->setStyleSheet("");
            toggleButton->setStyleSheet("");
        } });
    for (auto *barSet : barSets)
    {
        QObject::connect(barSet, &QBarSet::hovered, [barSet](bool status, int index)
                         {
                                 if (status)
                                 {
                                     QToolTip::showText(QCursor::pos(), QString::number((*barSet)[index]));
              } });
    }
    for (auto *barSet : ccSets)
    {
        QObject::connect(barSet, &QBarSet::hovered, [barSet](bool status, int index)
                         {
                                 if (status)
                                 {
                                     QToolTip::showText(QCursor::pos(), QString::number((*barSet)[index]));
                                } 
                        });
    }
    for (auto *barSet : tokenBars)
    {
        QObject::connect(barSet, &QBarSet::hovered, [barSet](bool status, int index)
                         {
                                 if (status)
                                 {
                                     QToolTip::showText(QCursor::pos(), QString::number((*barSet)[index]));
                                } 
                        });
    }
    QObject::connect(sizeSeries, &QBarSeries::clicked, [sizeSeries](int index, QBarSet *barSet)
                     {
            for (auto *set : sizeSeries->barSets()) {
                QColor color = set->color();
                color.setAlphaF(0.3);
                set->setColor(color);
            }
            QColor highlightColor = barSet->color();
            highlightColor.setAlphaF(1.0);
            barSet->setColor(highlightColor); 
        });
    QObject::connect(ccSeries, &QBarSeries::clicked, [ccSeries](int index, QBarSet *barSet)
                     {
    for (auto *set : ccSeries->barSets()) {
        QColor color = set->color();
        color.setAlphaF(0.3);
        set->setColor(color);
    }   
        QColor highlightColor = barSet->color();
        highlightColor.setAlphaF(1.0);
        barSet->setColor(highlightColor); 
    });
    QObject::connect(Tokens, &QBarSeries::clicked, [Tokens](int index, QBarSet *barSet)
                     {
    for (auto *set : Tokens->barSets()) {
        QColor color = set->color();
        color.setAlphaF(0.3);
        set->setColor(color);
    }   
        QColor highlightColor = barSet->color();
        highlightColor.setAlphaF(1.0);
        barSet->setColor(highlightColor); 
    });
    for (auto *Set : barSetsHalstead)
    {
        QObject::connect(Set, &QBarSet::hovered, [Set](bool status, int index)
                         {
                                 if (status)
                                {
                                     QToolTip::showText(QCursor::pos(), QString::number((*Set)[index]));
                                } 
                        });
    }

    QObject::connect(halsteadSeries, &QBarSeries::clicked, [halsteadSeries](int index, QBarSet *halsteadSets)
                     {
        for (auto *set : halsteadSeries->barSets()) {
            QColor color = set->color();
            color.setAlphaF(0.3);
            set->setColor(color);
        }
            QColor highlightColor = halsteadSets->color();
            highlightColor.setAlphaF(1.0);
            halsteadSets->setColor(highlightColor); 
        });
    switchChartButton->setFixedSize(200, 30);
    showHalsteadButton->setFixedSize(200, 30);
    showTokens->setFixedSize(170, 30);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(toggleButton);
    buttonLayout->addWidget(exportButton);
    buttonLayout->addStretch();
    buttonLayout->addWidget(exitButton);
    layout->addLayout(buttonLayout);

    toggleButton->setStyleSheet(
        "QPushButton { background-color: #007BFF; color: white; font-size: 14px; padding: 8px; border-radius: 5px; }"
        "QPushButton:hover { background-color:rgb(87, 91, 116); }");

    exportButton->setStyleSheet(
        "QPushButton { background-color: #28a745; color: white; font-size: 14px; padding: 8px; border-radius: 5px; }"
        "QPushButton:hover { background-color: #218838; }");

    exitButton->setStyleSheet(
        "QPushButton { background-color: #dc3545; color: white; font-size: 14px; padding: 8px; border-radius: 5px; }"
        "QPushButton:hover { background-color: #c82333; }");

    switchChartButton->setStyleSheet(
        "QPushButton { background-color:rgb(117, 102, 61); color: white; font-size: 14px; padding: 8px; border-radius: 5px; }"
        "QPushButton:hover { background-color:rgb(55, 71, 163); }");

    showHalsteadButton->setStyleSheet(
        "QPushButton { background-color:rgb(61, 109, 117); color: white; font-size: 14px; padding: 8px; border-radius: 5px; }"
        "QPushButton:hover { background-color:rgb(55, 71, 163); }");

    showTokens->setStyleSheet(
        "QPushButton { background-color:rgb(117, 61, 73); color: white; font-size: 14px; padding: 8px; border-radius: 5px; }"
        "QPushButton:hover { background-color:rgb(163, 55, 154); }");

    QHBoxLayout *metricsLayout = new QHBoxLayout();
    metricsLayout->addWidget(switchChartButton, 0, Qt::AlignLeft);
    metricsLayout->addStretch();
    metricsLayout->addWidget(showHalsteadButton, 0, Qt::AlignRight);
    metricsLayout->addWidget(showTokens, 0, Qt::AlignLeft);

    layout->addLayout(metricsLayout);
    layout->addWidget(chartView);

    centralWidget->setLayout(layout);
    window.setCentralWidget(centralWidget);
    window.resize(900, 600);
    window.show();

    app.exec();
}
