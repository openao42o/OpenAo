using System;
using System.IO;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Reflection;

using Library;
using Formatter.Omi;
using Formatter.Inf;
using Formatter.Dat;

namespace Library
{
    using Serializer = Action<string, object>;
    using Deserializer = Func<string, object>;

    public struct CachedInfo
    {
        public string Name { get; }
        public object Data => InternalData ?? (InternalData = Deserializer(Name));
        
        private Serializer Serializer { get; }
        private Deserializer Deserializer { get; }

        private object InternalData { get; set; }

        public CachedInfo(string name, Serializer serializer, Deserializer deserializer)
        {
            Name = name;

            Serializer = serializer;
            Deserializer = deserializer;
            InternalData = null;
        }

        public void Update(object data) => Serializer(Name, InternalData = data);
    }

    public class ResManager
    {
        public CachedInfo omiinfo { get; }

        public IEnumerable<CachedInfo> objectinfo { get; }
        public IEnumerable<CachedInfo> effectinfo { get; }
        public IEnumerable<CachedInfo> mapinfo { get; }


        void SerializeOmi(string name, object value)
        {
            using (var dest = File.OpenWrite(@"res-tex\" + name + ".tex"))
                FormatterOmi.Write(dest, value as object[]);
        }
        void SerializeObjectInfo(string name, object value)
        {
            using (var dest = File.OpenWrite(@"res-dev\objectinfo\" + name))
                ObjectInfoFormatter.Write(dest, value as object[]);
        }
        void SerializeEffectInfo(string name, object value)
        {
            using (var dest = File.OpenWrite(@"res-dev\effectinfo\" + name))
                EffectInfoFormatter.Write(dest, value as object[]);
        }
        void SerializeMapInfo(string name, object value)
        {
            throw new NotImplementedException();
        }

        object DeserializeOmi(string name)
        {
            using (var source = File.OpenRead(@"res-tex\" + name + ".tex"))
                return FormatterOmi.Read(source);
        }
        object DeserializeObjectInfo(string name)
        {
            using (var source = File.OpenRead(@"res-dev\objectinfo\" + name))
                return ObjectInfoFormatter.Read(source);
        }
        object DeserializeEffectInfo(string name)
        {
            using (var source = File.OpenRead(@"res-dev\effectinfo\" + name))
                return EffectInfoFormatter.Read(source);
        }
        object DeserializeMapInfo(string name)
        {
            using (AtumZip.ZippedFile zpf = File.OpenRead(@"res-map\" + name + ".dat"))
                return MapFormatter.Read(zpf.At(0));
        }


        public ResManager()
        {
            CheckResources();

            omiinfo = new CachedInfo("omi", SerializeOmi, DeserializeOmi);

            objectinfo = Directory.GetFiles(@"res-dev\objectinfo\")
                .Where(s => !Path.HasExtension(s))
                .Select(s => Path.GetFileNameWithoutExtension(s))
                .Select(s => new CachedInfo(s, SerializeObjectInfo, DeserializeObjectInfo));

            effectinfo = Directory.GetFiles(@"res-dev\effectinfo\")
                .Where(s => !Path.HasExtension(s))
                .Select(s => Path.GetFileNameWithoutExtension(s))
                .Select(s => new CachedInfo(s, SerializeEffectInfo, DeserializeEffectInfo));

            mapinfo = Directory.GetFiles(@"res-map\", "*.dat")
                .Select(s => Path.GetFileNameWithoutExtension(s))
                .Select(s => new CachedInfo(s, SerializeMapInfo, DeserializeMapInfo));
        }

        /// <summary>
        /// Checks if the unpacked resources exist in .\Res-Dev\
        /// and asks to create them when they don't.
        /// </summary>
        private void CheckResources()
        {
            // Could also put everything in arrays and just loop over them...

            var rescheck = new[]
            {
                !Directory.Exists(@"res-dev\objectinfo"),
                !Directory.Exists(@"res-dev\effectinfo"),
                !Directory.Exists(@"res-dev\spreff"),
                !Directory.Exists(@"res-dev\item"),
                !Directory.Exists(@"res-dev\bigitem"),
                !Directory.Exists(@"res-dev\sound"),
            };

            if (rescheck.Any(x => x == true) &&
                MessageBox.Show("Resources are not prepared. Would you like to prepare them now?",
                "Warning", MessageBoxButtons.YesNo) == DialogResult.Yes)
            {
                if (rescheck[0]) AtumZip.ZippedFile.UnzipFolder(@"res-dev", @"res-eff\objectinfo.inf");
                if (rescheck[1]) AtumZip.ZippedFile.UnzipFolder(@"res-dev", @"res-eff\effectinfo.inf");
                if (rescheck[2]) AtumZip.ZippedFile.UnzipFolder(@"res-dev", @"res-tex\spreff.tex");
                if (rescheck[3]) AtumZip.ZippedFile.UnzipFolder(@"res-dev", @"res-tex\item.tex");
                if (rescheck[4]) AtumZip.ZippedFile.UnzipFolder(@"res-dev", @"res-tex\bigitem.tex");
                if (rescheck[5]) AtumZip.ZippedFile.UnzipFolder(@"res-dev", @"res-snd\sound.dat");
            }
        }
    }
}
