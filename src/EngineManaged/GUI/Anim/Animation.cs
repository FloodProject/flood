using Flood.GUI.Controls;
using System;
using System.Collections.Generic;

namespace Flood.GUI.Anim
{
    public class Animation
    {
        protected Control m_Control;

        //private static List<Animation> g_AnimationsListed = new List<Animation>(); // unused
        private static readonly Dictionary<Control, List<Animation>> m_Animations = new Dictionary<Control, List<Animation>>();

        protected virtual void Think()
        {
            
        }

        public virtual bool Finished
        {
            get { throw new InvalidOperationException("Pure virtual function call"); }
        }

        public static void Add(Control control, Animation animation)
        {
            animation.m_Control = control;
            if (!m_Animations.ContainsKey(control))
                m_Animations[control] = new List<Animation>();
            m_Animations[control].Add(animation);
        }

        public static void Cancel(Control control)
        {
            if (m_Animations.ContainsKey(control))
            {
                m_Animations[control].Clear();
                m_Animations.Remove(control);
            }
        }

        internal static void GlobalThink()
        {
            foreach (KeyValuePair<Control, List<Animation>> pair in m_Animations)
            {
                var valCopy = pair.Value.FindAll(x =>true); // list copy so foreach won't break when we remove elements
                foreach (Animation animation in valCopy)
                {
                    animation.Think();
                    if (animation.Finished)
                    {
                        pair.Value.Remove(animation);
                    }
                }
            }
        }
    }
}
