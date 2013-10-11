using Flood.Remoting.Serialization;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Threading;

namespace Flood.Remoting
{
    public class ReferenceManager
    {
        private class Subscription
        {
            public RemotingPeer Peer;
            public int RemoteId;

            public Subscription(RemotingPeer peer, int remoteId)
            {
                Peer = peer;
                RemoteId = remoteId;
            }

            public override int GetHashCode()
            {
                return Peer.GetHashCode() ^ RemoteId.GetHashCode();
            }

            public override bool Equals(object obj)
            {
                var subscription = obj as Subscription;
                if (subscription == null)
                    return false;

                return subscription.RemoteId == RemoteId
                       && subscription.Peer == Peer;
            }
        }

        private class Reference
        {
            public int LocalId;
            public IObservableDataObject DataObject;

            public HashSet<RemotingPeer> Subscribers;

            public Subscription Subscription;

            public Reference(IObservableDataObject dataObject, int localId)
            {
                DataObject = dataObject;
                LocalId = localId;
            }
        }

        private Dictionary<int, Reference> localIdToReference;
        private Dictionary<IObservableDataObject, Reference> dataObjectToReference;
        private Dictionary<Subscription, Reference> subscriptionToReference;

        private int localIdCounter;

        private HashSet<Reference> referencesChanged;

        private RemotingManager remotingManager;

        public ReferenceManager(RemotingManager remotingManager)
        {
            referencesChanged = new HashSet<Reference>();

            localIdToReference = new Dictionary<int, Reference>();
            dataObjectToReference = new Dictionary<IObservableDataObject, Reference>();
            subscriptionToReference = new Dictionary<Subscription, Reference>();

            this.remotingManager = remotingManager;
        }

        public void Process(Message data)
        {
            switch (data.Header.CallType)
            {
                case MessageType.ReferenceChanges:
                    ProcessChanges(data);
                    return;
                case MessageType.ReferenceSubscribe:
                    ProcessSubscribe(data);
                    return;
                case MessageType.ReferenceUnsubscribe:
                    ProcessUnsubscribe(data);
                    return;
                default:
                    Debug.Assert(false);
                    return;
            }
        }

        private void ProcessChanges(Message data)
        {
            var subscription = new Subscription(data.Peer, data.Header.RemoteId);
            Reference reference;
            if (!subscriptionToReference.TryGetValue(subscription, out reference))
                throw new Exception("Reference not found.");

            reference.DataObject.Read(data);
        }

        private void ProcessSubscribe(Message data)
        {
            var localId = data.Header.LocalId;

            Reference reference;
            if (!localIdToReference.TryGetValue(localId, out reference))
                throw new Exception("No reference to subscribe to.");

            if (reference.Subscribers == null)
                reference.Subscribers = new HashSet<RemotingPeer>();

            reference.Subscribers.Add(data.Peer);
            //TODO send complete serialization
        }

        private void ProcessUnsubscribe(Message data)
        {
            var localId = data.Header.LocalId;

            Reference reference;
            if (!localIdToReference.TryGetValue(localId, out reference))
                throw new Exception("No reference to subscribe to.");

            reference.Subscribers.Remove(data.Peer);
        }

        public unsafe void DispatchChanges()
        {
            foreach (var reference in referencesChanged)
            {
                var bitFieldsCount = reference.DataObject.BaseDataObjectCount + 1;
                var bitFields = stackalloc BitField[bitFieldsCount];

                reference.DataObject.GetResetChanges(bitFields);

                foreach (var peer in reference.Subscribers)
                {
                    var peerData = new Message(peer, remotingManager, reference.LocalId, 0, MessageType.ReferenceChanges);
                    // TODO: Optimize this. Dont't serialize again for each peer.
                    reference.DataObject.Write(peerData, bitFields, bitFieldsCount);
                    peerData.Dispatch();
                }
            }
        }

        private Reference CreateReference(IObservableDataObject dataObject)
        {
            var localId = Interlocked.Increment(ref localIdCounter);
            var reference = new Reference(dataObject, localId);

            localIdToReference.Add(localId, reference);
            dataObjectToReference.Add(dataObject, reference);

            return reference;
        }

        public void Publish(IObservableDataObject dataObject)
        {
            if (dataObjectToReference.ContainsKey(dataObject))
                return;

            var reference = CreateReference(dataObject);

            dataObject.PropertyChanged += (type, i) => referencesChanged.Add(reference);
        }

        public void Subscribe(IObservableDataObject dataObject, RemotingPeer peer, int remoteId)
        {
            Reference reference;
            if (!dataObjectToReference.TryGetValue(dataObject, out reference))
                reference = CreateReference(dataObject);

            reference.Subscription = new Subscription(peer, remoteId);

            subscriptionToReference.Add(reference.Subscription, reference);

            var data = new Message(peer, remotingManager, reference.LocalId, remoteId, MessageType.ReferenceSubscribe);
            data.Dispatch();
        }

        public bool TryGetLocalId(object obj, out int localId)
        {
            localId = 0;

            var dataObject = obj as IObservableDataObject;
            if (dataObject == null)
                return false;

            Reference reference;
            if (!dataObjectToReference.TryGetValue(dataObject, out reference))
                return false;

            localId = reference.LocalId;
            return true;
        }

        public static bool Subscribe(object obj)
        {
            var reference = obj as IDataObjectReference;
            var observable = obj as IObservableDataObject;
            if (reference == null || observable == null)
                return false;

            reference.ReferenceManager.Subscribe(observable, reference.Peer, reference.RemoteId);
            return true;
        }

        public static bool Publish(object obj)
        {
            var dataObject = obj as IObservableDataObject;
            if (dataObject == null)
                return false;

            dataObject.IsReference = true;

            return true;
        }
    }
}
