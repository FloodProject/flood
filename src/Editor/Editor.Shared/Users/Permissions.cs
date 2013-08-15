using System;
using System.Collections.Generic;
using Flood.RPC.Metadata;

namespace Flood.Editor.Shared
{
    [DataObject("FA7AE0C8-9D8E-4308-A7F0-302E1F74785B")]
    public class Permissions
    {
        [Id(0)]
        public bool WriteFiles { get; set; }

        [Id(1)]
        public bool AddRemoveFiles { get; set; }

        [Id(2)]
        public bool ReadFiles { get; set; }

        [Id(3)]
        public Dictionary<UserType, bool> EditPermissions { get; set; }

        [Id(4)]
        public Dictionary<UserType, bool> AddRemoveUsers { get; set; }

        public Permissions()
        {
            EditPermissions = new Dictionary<UserType, bool>();
            AddRemoveUsers = new Dictionary<UserType, bool>();

            foreach (UserType usertype in Enum.GetValues(typeof(UserType)))
            {
                EditPermissions[usertype] = false;
                AddRemoveUsers[usertype] = false;
            }
        }

        public void AllPermissions()
        {
            WriteFiles = true;
            AddRemoveFiles = true;
            ReadFiles = true;

            foreach (UserType usertype in Enum.GetValues(typeof(UserType)))
            {
                EditPermissions[usertype] = true;
                AddRemoveUsers[usertype] = true;
            }
        }

        public void ZeroPermissions()
        {
            WriteFiles = false;
            AddRemoveFiles = false;
            ReadFiles = false;

            foreach (UserType usertype in Enum.GetValues(typeof(UserType)))
            {
                EditPermissions[usertype] = false;
                AddRemoveUsers[usertype] = false;
            }
        }

        public void ObserverPermissions()
        {
            WriteFiles = false;
            AddRemoveFiles = false;
            ReadFiles = true;

            foreach (UserType usertype in Enum.GetValues(typeof(UserType)))
            {
                EditPermissions[usertype] = false;
                AddRemoveUsers[usertype] = false;
            }
        }

        public void OwnerPermissions()
        {
            WriteFiles = true;
            AddRemoveFiles = true;
            ReadFiles = true;

            foreach (UserType usertype in Enum.GetValues(typeof(UserType)))
            {
                EditPermissions[usertype] = true;
                AddRemoveUsers[usertype] = true;
            }

            EditPermissions[UserType.Owner] = false;
            AddRemoveUsers[UserType.Owner] = false;
        }

        public void MemberPermissions()
        {
            WriteFiles = true;
            AddRemoveFiles = true;
            ReadFiles = true;

            foreach (UserType usertype in Enum.GetValues(typeof(UserType)))
            {
                EditPermissions[usertype] = false;
                AddRemoveUsers[usertype] = false;
            }
        }




    }
}
