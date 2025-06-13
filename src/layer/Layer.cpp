#include "Layer.hpp"

#include "../history/CanvasHistoryManager.hpp"
#include "../tools/BrushTool.hpp"
#include "../tools/ColorPicker.hpp"
#include "../tools/EraseTool.hpp"
#include "../tools/FillTool.hpp"
#include "../tools/LineTool.hpp"
#include "../tools/MoveTool.hpp"
#include "../tools/SelectionTool.hpp"
#include "../tools/ShapeTool.hpp"
#include "../tools/ToolSettings.hpp"

namespace harmoniq { namespace layer {

Layer::Layer(QQuickItem *parent) : QQuickPaintedItem(parent)
{
    setAcceptedMouseButtons(Qt::LeftButton);
    setAcceptHoverEvents(true);
    setAcceptTouchEvents(true);
    setFlag(QQuickItem::ItemHasContents);
    setFocusPolicy(Qt::StrongFocus);

    qDebug() << "LAYER: " << this;
}

Layer::~Layer() { qDebug() << "DESTROY LAYER: " << this; }

void Layer::updateCanvasSize()
{
    if (width() <= 0 || height() <= 0)
        return;

    if (!m_initialized || m_canvas.size() != QSize(width(), height())) {
        m_canvas = QImage(width(), height(), QImage::Format_ARGB32);
        m_canvas.fill(m_backgroundColor.isValid() ? m_backgroundColor : Qt::transparent);
        m_initialized = true;

        if (m_toolHandler) {
            m_toolHandler->setImage(&m_canvas);
        }

        update();
    }
}

void Layer::geometryChange(const QRectF &newGeometry, const QRectF &oldGeometry)
{
    QQuickPaintedItem::geometryChange(newGeometry, oldGeometry);
    updateCanvasSize();
}

void Layer::paint(QPainter *painter)
{
    if (m_visible) {
        if (!m_initialized)
            updateCanvasSize();
        painter->drawImage(0, 0, m_canvas);
        if (m_currentSelection.isValid()) {
            QPen pen(Qt::DashLine);
            pen.setColor(Qt::black);
            painter->setPen(pen);
            painter->drawRect(m_currentSelection);
        }
    }
}

void Layer::mousePressEvent(QMouseEvent *event)
{
    qDebug() << "Layer::mousePressEvent for layer:" << this << "isActive:" << m_active;
    if (m_locked || !m_active)
        return;

    if (m_toolHandler) {
        m_toolHandler->press(event);
        update();
    }
    emit stateChanged(m_canvas, this);
}

void Layer::mouseMoveEvent(QMouseEvent *event)
{
    if (m_locked || !m_active)
        return;

    if (m_toolHandler) {
        ToolInfo info;
        info.size = ToolSettings::instance()->size();
        info.color = ToolSettings::instance()->color();
        info.hardness = ToolSettings::instance()->hardness();
        info.opacity = ToolSettings::instance()->opacity();
        info.fill = ToolSettings::instance()->fill();
        info.selection = m_currentSelection;
        m_toolHandler->setToolInfo(info);

        m_toolHandler->move(event);
        update();
    }
}

void Layer::mouseReleaseEvent(QMouseEvent *event)
{
    if (m_locked || !m_active)
        return;

    if (m_toolHandler) {
        m_toolHandler->release(event);
        emit stateChanged(m_canvas, this);
        update();
    }
}

void Layer::keyPressEvent(QKeyEvent *event)
{
    qDebug() << "Key pressed in Layer:" << event->key() << "Modifiers:" << event->modifiers();
    if (event->key() == Qt::Key_Escape) {
        clearSelection();
    }
    QQuickPaintedItem::keyPressEvent(event);
}

void Layer::setBackgroundColor(const QColor &color)
{
    if (m_backgroundColor != color) {
        m_backgroundColor = color;
        if (m_initialized) {
            m_canvas.fill(m_backgroundColor);
        }
        emit backgroundColorChanged();
        update();
    }
}

void Layer::setActive(const bool &active)
{
    if (m_active != active) {
        m_active = active;
        setAcceptedMouseButtons(active ? Qt::LeftButton : Qt::NoButton);
        emit activeChanged();
    }
}

void Layer::setVisibility(const bool &visible)
{
    if (m_visible != visible) {
        m_visible = visible;
        emit visibleUpdated(visible);
        update();
    }
}

void Layer::setLocked(const bool &locked)
{
    if (m_locked != locked) {
        m_locked = locked;
        emit lockedChanged();
        update();
    }
}

void Layer::setImage(const QImage &image)
{
    if (m_canvas != image) {
        m_canvas = image;
        update();
    }
}

void Layer::setCurrentTool(ToolType type)
{
    if (m_currentTool != type) {
        m_currentTool = type;

        switch (m_currentTool) {
        case ToolType::Brush:
            m_toolHandler = std::make_unique<BrushTool>();
            break;
        case ToolType::Eraser:
            m_toolHandler = std::make_unique<EraseTool>();
            break;
        case ToolType::Fill:
            m_toolHandler = std::make_unique<FillTool>();
            break;
        case ToolType::Picker:
            m_toolHandler = std::make_unique<ColorPickerTool>();
            break;
        case ToolType::Shape:
            m_toolHandler = std::make_unique<ShapeTool>();
            break;
        case ToolType::Line:
            m_toolHandler = std::make_unique<LineTool>();
            break;
        case ToolType::Move:
            m_toolHandler = std::make_unique<MoveTool>();
            break;
        case ToolType::Selection:
            m_toolHandler = std::make_unique<SelectionTool>();
            break;
        default:
            m_toolHandler.reset();
            break;
        }

        m_toolHandler->setImage(&m_canvas);

        if (auto *selectionTool = dynamic_cast<SelectionTool *>(m_toolHandler.get())) {
            selectionTool->setLayer(this);
        }

        ToolInfo info;
        info.color = ToolSettings::instance()->color();
        info.size = ToolSettings::instance()->size();
        info.opacity = ToolSettings::instance()->opacity();
        info.hardness = ToolSettings::instance()->hardness();
        info.fill = ToolSettings::instance()->fill();
        info.selection = m_currentSelection;

        m_toolHandler->setToolInfo(info);

        qDebug() << "Tool changed to:" << m_currentTool << "for layer:" << this;
        update();
    }
}

void Layer::setSelection(const QRect &rect)
{
    m_currentSelection = rect;
    update();
}

void Layer::clearSelection()
{
    m_currentSelection = QRect();
    update();
}
}} // namespace harmoniq::layer
