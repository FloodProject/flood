using EditorManaged_d.EngineManaged;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace EditorManaged_d
{
    class Entity
    {
        private List<Entity> childs;
        private List<Component> components;

        public Entity Parent{ get; private set; }

        public String Name { get; private set; }

        public EntityId Id { get; private set; }

        public  ReadOnlyCollection<Entity> Childs { 
            get; private set;
        }


        public  ReadOnlyCollection<Component> Components { 
            get; private set;
        }


        public event Action<Entity> EntityAdd;
        public event Action<Entity> EntityRemove;
        public event Action<Component> ComponentAdd;
        public event Action<Component> ComponentRemove;


        public Entity(){
            childs = new List<Entity>();
            Childs = new ReadOnlyCollection<Entity>(childs);
            components = new List<Component>();
            Components = new ReadOnlyCollection<Component>(components);
        }


        protected void AddChild(Entity child){
            childs.Add(child);
            if(EntityAdd!=null)
                EntityAdd.Invoke(child);
        }


    }
}
