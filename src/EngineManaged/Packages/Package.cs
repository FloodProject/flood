using System;
using Flood.Remoting.Metadata;
using System.Collections.Generic;

namespace Flood.Packages
{
    public struct PackageVersion
    {
        /// Major is incremented when:
        ///     -Existing Message field type is changed or removed.
        ///     -Existing RPC method parameter type is changed or removed.
        ///     -Existing RPC method return type is changed.
        ///     -Existing resource is moved or removed.
        public int Major;

        /// Minor is incremented when:
        ///     -New Messages or Services are added.
        ///     -New Parameter are added to an existing RPC method.
        ///     -New fields are added to an existing Message.
        ///     -New resources are added.
        public int Minor;

        public int Patch;

        public PackageVersion(Version version)
            :this(version.Major, version.Minor, version.Build)
        {
        }

        public PackageVersion(int major, int minor, int patch)
        {
            Major = major;
            Minor = minor;
            Patch = patch;
        }

        public PackageVersion(string version)
        {
            var split = version.Split('.');
            Major = int.Parse(split[0]);
            Minor = int.Parse(split[1]);
            Patch = int.Parse(split[2]);
        }

        public override string ToString()
        {
            return string.Format("{0}.{1}.{2}", Major, Minor, Patch);
        }
    }

    public struct PackageName
    {
        public string Id;

        public PackageVersion Version;

        public PackageName(string id, PackageVersion version)
        {
            Id = id;
            Version = version;
        }

        public PackageName(string packageName)
        {
            var split = packageName.Split(new []{'.'}, 2);
            Id = split[0];
            Version = new PackageVersion(split[1]);
        }

        public override string ToString()
        {
            return string.Format("{0}.{1}", Id, Version);
        }
    }

    public class Package
    {
        public IList<PackageName> Dependencies;

        /// The unique identifier for the package.
        public string Id;

        /// The version of the package
        public PackageVersion Version;

        /// The human-friendly title of the package
        public string Title;

        public IList<string> Authors;

        public string Description;

        public string ReleaseNotes;
    }
}
