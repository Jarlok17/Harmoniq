#pragma once

#include "../config/precompiled.hpp"
#include "../layer/Layer.hpp" // Для доступу до Layer

namespace harmoniq {

class CanvasHistoryManager : public QObject
{
        Q_OBJECT

    public:
        explicit CanvasHistoryManager(QObject *parent = nullptr);

        struct HistoryState
        {
                QImage image;
                layer::Layer *layer;
        };

        Q_INVOKABLE bool canUndo() const { return !undoStack.isEmpty(); }
        Q_INVOKABLE bool canRedo() const { return !redoStack.isEmpty(); }

        Q_INVOKABLE void undo();
        Q_INVOKABLE void redo();

    public slots:
        void saveState(const QImage &image, layer::Layer *layer);

    signals:
        void stateChanged();

    private:
        QStack<HistoryState> undoStack;
        QStack<HistoryState> redoStack;
};

} // namespace harmoniq
