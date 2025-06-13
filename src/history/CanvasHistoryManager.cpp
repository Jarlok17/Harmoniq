#include "CanvasHistoryManager.hpp"

namespace harmoniq {

CanvasHistoryManager::CanvasHistoryManager(QObject *parent) : QObject(parent) {}

void CanvasHistoryManager::saveState(const QImage &image, layer::Layer *layer)
{
    if (!layer || !layer->isActive())
        return;

    HistoryState state;
    state.image = image.copy();
    state.layer = layer;
    undoStack.push(state);
    redoStack.clear();
    qDebug() << "State saved. Undo size:" << undoStack.size() << "Layer:" << layer << "Image size:" << image.size();
    emit stateChanged();
}

void CanvasHistoryManager::undo()
{
    if (canUndo()) {
        HistoryState state = undoStack.pop();
        redoStack.push(state);
        qDebug() << "Applying undo to layer:" << state.layer << "with image size:" << state.image.size();
        state.layer->setImage(state.image);
        state.layer->update();
        qDebug() << "Undo performed. Undo stack size:" << undoStack.size() << "Redo stack size:" << redoStack.size();
        emit stateChanged();
    } else {
        qWarning() << "No state to undo!";
    }
}

void CanvasHistoryManager::redo()
{
    if (canRedo()) {
        HistoryState state = redoStack.pop();
        undoStack.push(state);
        state.layer->setImage(state.image);
        state.layer->update();
        qDebug() << "Redo performed. Undo stack size:" << undoStack.size() << "Redo stack size:" << redoStack.size();
        emit stateChanged();
    } else {
        qWarning() << "No state to redo!";
    }
}

} // namespace harmoniq
