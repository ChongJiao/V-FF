#pragma once 
#include "Layer.hpp"

namespace  VFF
{
    class ImGuiLayer: public Layer{
        public:
            ImGuiLayer();
            virtual ~ImGuiLayer() = default;
            virtual void Attach() override;
            virtual void Detach() override;
            virtual void Update() override;

            void Begin();
            void End();
        private:
            bool m_ShowDemo = true;


    };
    
} // namespace  VFF
