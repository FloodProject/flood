
namespace EngineManaged.GUI.Renderer
{
    public interface ICacheToTexture
    {
        void Initialize();
        void ShutDown();

        /// <summary>
        /// Called to set the target up for rendering.
        /// </summary>
        /// <param name="control">Control to be rendered.</param>
        void SetupCacheTexture(Controls.Base control);

        /// <summary>
        /// Called when cached rendering is done.
        /// </summary>
        /// <param name="control">Control to be rendered.</param>
        void FinishCacheTexture(Controls.Base control);

        /// <summary>
        /// Called when gwen wants to draw the cached version of the control. 
        /// </summary>
        /// <param name="control">Control to be rendered.</param>
        void DrawCachedControlTexture(Controls.Base control);

        /// <summary>
        /// Called to actually create a cached texture. 
        /// </summary>
        /// <param name="control">Control to be rendered.</param>
        void CreateControlCacheTexture(Controls.Base control);
        
        void UpdateControlCacheTexture(Controls.Base control);
        void SetRenderer(Base renderer);
    }
}
