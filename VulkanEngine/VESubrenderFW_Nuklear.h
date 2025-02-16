/**
* The Vienna Vulkan Engine
*
* (c) bei Helmut Hlavacs, University of Vienna
*
*/

#ifndef VESUBRENDERFWNUKLEAR_H
#define VESUBRENDERFWNUKLEAR_H


namespace ve {

	/**
	* \brief Subrenderer that manages entities that have only one color
	*/
	class VESubrenderFW_Nuklear : public VESubrenderFW {
	protected:
		struct nk_context *m_ctx;						///<The Nuklear context storing the GUI data

	public:
		///Constructor of class VESubrenderFW_Nuklear
		VESubrenderFW_Nuklear() {};
		///Destructor of class VESubrenderFW_Nuklear
		virtual ~VESubrenderFW_Nuklear() {};

		///\returns the class of the subrenderer
		virtual veSubrenderClass getClass() { return VE_SUBRENDERER_CLASS_OVERLAY; };
		///\returns the type of the subrenderer
		virtual veSubrenderType getType() { return VE_SUBRENDERER_TYPE_NUKLEAR; };

		virtual void initSubrenderer();
		virtual void closeSubrenderer();
		virtual void prepareDraw();
		virtual void draw(	VkCommandBuffer commandBuffer, uint32_t imageIndex, uint32_t numPass,
							VECamera *pCamera, VELight *pLight,
							std::vector<VkDescriptorSet> descriptorSetsShadow) {};
		virtual VkSemaphore draw(uint32_t imageIndex, VkSemaphore wait_semaphore);

		///\returns the Nuklear context
		virtual struct nk_context *getContext() { return m_ctx; };
		///\returns texture handle
		void *addTexture(VETexture* texture);

	};
}



#endif
