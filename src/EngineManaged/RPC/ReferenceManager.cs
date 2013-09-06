﻿using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Threading;
using Flood.RPC.Serialization;

namespace Flood.RPC
{
    public class ReferenceManager
    {
        private class Subscription
        {
            public RPCPeer Peer;
            public int RemoteId;

            public Subscription(RPCPeer peer, int remoteId)
            {
                Peer = peer;
                RemoteId = remoteId;
            }
        }

        private class Reference
        {
            public int LocalId;
            public IObservableDataObject DataObject;

            public HashSet<RPCPeer> Subscribers;

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

        public ReferenceManager()
        {
            referencesChanged = new HashSet<Reference>();

            localIdToReference = new Dictionary<int, Reference>();
            dataObjectToReference = new Dictionary<IObservableDataObject, Reference>();
            subscriptionToReference = new Dictionary<Subscription, Reference>();
        }

        public void Process(RPCData data)
        {
            switch (data.Header.CallType)
            {
                case RPCDataType.ReferenceChanges:
                    ProcessChanges(data);
                    return;
                case RPCDataType.ReferenceSubscribe:
                    ProcessSubscribe(data);
                    return;
                case RPCDataType.ReferenceUnsubscribe:
                    ProcessUnsubscribe(data);
                    return;
                default:
                    Debug.Assert(false);
                    return;
            }
        }

        private void ProcessChanges(RPCData data)
        {
            var subscription = new Subscription(data.Peer, data.Header.RemoteId);
            Reference reference;
            if(!subscriptionToReference.TryGetValue(subscription, out reference))
                throw new Exception("Reference not found.");

            reference.DataObject.Read(data);
        }

        private void ProcessSubscribe(RPCData data)
        {
            var localId = data.Header.LocalId;

            Reference reference;
            if(!localIdToReference.TryGetValue(localId, out reference))
                throw new Exception("No reference to subscribe to.");

            if(reference.Subscribers == null)
                reference.Subscribers = new HashSet<RPCPeer>();

            reference.Subscribers.Add(data.Peer);
            //TODO send complete serialization
        }

        private void ProcessUnsubscribe(RPCData data)
        {
            var localId = data.Header.LocalId;

            Reference reference;
            if(!localIdToReference.TryGetValue(localId, out reference))
                throw new Exception("No reference to subscribe to.");

            reference.Subscribers.Remove(data.Peer);
        }

        public void DispatchChanges()
        {
            foreach (var reference in referencesChanged)
            {
                var data = new RPCData();
                var changes = reference.DataObject.GetResetChanges();
                reference.DataObject.Write(data, changes);

                foreach (var peer in reference.Subscribers)
                {
                    var peerData = RPCData.Create(peer, reference.LocalId, 0, RPCDataType.ReferenceChanges);
                    data.Serializer.Buffer.CopyTo(peerData.Serializer.Buffer);
                    data.Dispatch();
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

            var reference =  CreateReference(dataObject);

            dataObject.PropertyChanged += i => referencesChanged.Add(reference);
        }

        public void Subscribe(IObservableDataObject dataObject, RPCPeer peer, int remoteId)
        {
            Reference reference;
            if (!dataObjectToReference.TryGetValue(dataObject, out reference))
                reference = CreateReference(dataObject);

            reference.Subscription = new Subscription(peer ,remoteId);

            subscriptionToReference.Add(reference.Subscription, reference);

            var data = RPCData.Create(peer, reference.LocalId, remoteId, RPCDataType.ReferenceSubscribe);
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
            var dataObject= obj as IDataObjectReference;
            if (dataObject == null)
                return false;

            dataObject.ReferenceManager.Subscribe(dataObject, dataObject.Peer, dataObject.RemoteId);
            return true;
        }

        public static bool Publish(object obj)
        {
            var dataObject= obj as IObservableDataObject;
            if (dataObject == null)
                return false;

            dataObject.IsReference = true;

            return true;
        }
    }
}
