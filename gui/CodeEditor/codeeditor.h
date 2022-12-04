#ifndef COMPILER_CODEEDITOR_H
#define COMPILER_CODEEDITOR_H

#include <QPlainTextEdit>

QT_BEGIN_NAMESPACE
class QPaintEvent;

class QResizeEvent;

class QSize;

class QWidget;

QT_END_NAMESPACE

class LineNumberArea;

class CodeEditor : public QPlainTextEdit {
Q_OBJECT

public:
    explicit CodeEditor(QWidget *parent = nullptr);

    void lineNumberAreaPaintEvent(QPaintEvent *event);

    int lineNumberAreaWidth();

protected:
    void resizeEvent(QResizeEvent *event) override;

private slots:

    void updateLineNumberAreaWidth(int newBlockCount);

    void highlightCurrentLine();

    void updateLineNumberArea(const QRect &rect, int dy);

private:
    QWidget *lineNumberArea;
};

class LineNumberArea : public QWidget {
public:
    explicit LineNumberArea(CodeEditor *editor) : QWidget(editor), codeEditor(editor) {}

    [[nodiscard]] QSize sizeHint() const override {
        return {codeEditor->lineNumberAreaWidth(), 0};
    }

protected:
    void paintEvent(QPaintEvent *event) override {
        codeEditor->lineNumberAreaPaintEvent(event);
    }

private:
    CodeEditor *codeEditor;
};


#endif //COMPILER_CODEEDITOR_H