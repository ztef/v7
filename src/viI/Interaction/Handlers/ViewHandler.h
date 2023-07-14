
#include <vsg/all.h>


class ViewHandler : public vsg::Inherit<vsg::Visitor, ViewHandler>
{
public:
    vsg::ref_ptr<vsg::RenderGraph> renderGraph;

    ViewHandler(vsg::ref_ptr<vsg::RenderGraph> in_renderGrah) : renderGraph(in_renderGrah) {}

    void apply(vsg::KeyPressEvent &keyPress) override
    {
        if (keyPress.keyBase == 's')
        {

            // Lee las Views

            std::vector<std::pair<size_t, vsg::ref_ptr<vsg::View>>> views;
            for (size_t i = 0; i < renderGraph->children.size(); ++i)
            {
                if (auto view = renderGraph->children[i].cast<vsg::View>())
                    views.emplace_back(i, view);
            }

            // si solo hay una vista no hace nada
            if (views.size() < 2)
                return;

            auto renderPass = renderGraph->getRenderPass();
            if (!renderPass)
                return;

            // Calcula extent de la vista 0
            auto view0 = views[0].second;
            auto viewport0 = view0->camera->getViewport();
            VkExtent2D extent0{static_cast<uint32_t>(viewport0.width), static_cast<uint32_t>(viewport0.height)};

            // Cakcula extent de la vista 1
            auto view1 = views[1].second;
            auto viewport1 = view1->camera->getViewport();
            VkExtent2D extent1{static_cast<uint32_t>(viewport1.width), static_cast<uint32_t>(viewport1.height)};

            // Realiza un SWAP de lugar en los children del renderGraph :

            // swap rendering order by swap the view entries in the renderGraph->children
            std::swap(renderGraph->children[views[0].first], renderGraph->children[views[1].first]);
            std::swap(views[0].second->camera->viewportState, views[1].second->camera->viewportState);

            // Intercambia las matrices de proyeccion :

            // change the aspect ratios of the projection matrices to fit the new diemensions.
            view0->camera->projectionMatrix->changeExtent(extent0, extent1);
            view1->camera->projectionMatrix->changeExtent(extent1, extent0);

            // Hace un WAIT

            // wait until the device is idle to avoid changing state while it's being used.
            vkDeviceWaitIdle(*(renderPass->device));

            // Actualiza PipeLines :

            vsg::UpdateGraphicsPipelines updateGraphicsPipelines;
            updateGraphicsPipelines.context = vsg::Context::create(renderPass->device);
            updateGraphicsPipelines.context->renderPass = renderPass;

            renderGraph->accept(updateGraphicsPipelines);
        }
    }
};
