using System;
using System.IO;
using System.Data;
using System.Linq;
using System.Collections.Generic;
using Library;

namespace Formatter
{
    // The Field classes are helpers to define
    // how serialization happens in Struct classes

    #region Field classes

    public interface IField
    {
        string Name { get; }

        uint Offset { get; }

        uint Length { get; }

        Type DataType { get; }

        object Read(byte[] source, uint offset);
        void Write(byte[] dest, uint offset, object value);
    }


    public class BinaryField : IField
    {
        public virtual string Name { get; }

        public virtual uint Offset { get; }

        public virtual uint Length { get; }

        public virtual Type DataType => typeof(byte[]);

        public BinaryField(string name, uint offset, uint length)
        {
            Name = name;
            Offset = offset;
            Length = length;
        }

        public virtual object Read(byte[] source, uint offset)
        {
            var buffer = new byte[Length];

            Array.Copy(source, offset, buffer, 0, Length);

            return buffer;
        }

        public virtual void Write(byte[] dest, uint offset, object value)
        {
            Array.Copy(value as byte[], 0, dest, offset, Length);
        }
    }

    public class Bool32Field : BinaryField
    {
        public override Type DataType { get { return typeof(bool); } }

        public Bool32Field(string name, uint offset) : base(name, offset, 4) { }

        public override object Read(byte[] source, uint offset)
        {
            return BitConverter.ToInt32(source, (int)offset) != 0;
        }
        public override void Write(byte[] dest, uint offset, object value)
        {
            BitConverter<int>.GetBytes(dest, ((bool)value) ? 1 : 0, (int)offset);
        }
    }

    public class SInt8Field : BinaryField
    {
        public override Type DataType { get { return typeof(sbyte); } }

        public SInt8Field(string name, uint offset) : base(name, offset, 1) { }

        public override object Read(byte[] source, uint offset)
        {
            return (sbyte)source[offset];
        }
        public override void Write(byte[] dest, uint offset, object value)
        {
            dest[offset] = (byte)(sbyte)value;
        }
    }
    public class SInt16Field : BinaryField
    {
        public override Type DataType { get { return typeof(short); } }

        public SInt16Field(string name, uint offset) : base(name, offset, 2) { }

        public override object Read(byte[] source, uint offset)
        {
            return BitConverter.ToInt16(source, (int)offset);
        }
        public override void Write(byte[] dest, uint offset, object value)
        {
            BitConverter<short>.GetBytes(dest, value, (int)offset);
        }
    }
    public class SInt32Field : BinaryField
    {
        public override Type DataType { get { return typeof(int); } }

        public SInt32Field(string name, uint offset) : base(name, offset, 4) { }

        public override object Read(byte[] source, uint offset)
        {
            return BitConverter.ToInt32(source, (int)offset);
        }
        public override void Write(byte[] dest, uint offset, object value)
        {
            BitConverter<int>.GetBytes(dest, value, (int)offset);
        }
    }
    public class SInt64Field : BinaryField
    {
        public override Type DataType { get { return typeof(long); } }

        public SInt64Field(string name, uint offset) : base(name, offset, 8) { }

        public override object Read(byte[] source, uint offset)
        {
            return BitConverter.ToInt64(source, (int)offset);
        }
        public override void Write(byte[] dest, uint offset, object value)
        {
            BitConverter<long>.GetBytes(dest, value, (int)offset);
        }
    }

    public class UInt8Field : BinaryField
    {
        public override Type DataType { get { return typeof(byte); } }

        public UInt8Field(string name, uint offset) : base(name, offset, 1) { }

        public override object Read(byte[] source, uint offset)
        {
            return source[offset];
        }
        public override void Write(byte[] dest, uint offset, object value)
        {
            dest[offset] = (byte)value;
        }
    }
    public class UInt16Field : BinaryField
    {
        public override Type DataType { get { return typeof(ushort); } }

        public UInt16Field(string name, uint offset) : base(name, offset, 2) { }

        public override object Read(byte[] source, uint offset)
        {
            return BitConverter.ToUInt16(source, (int)offset);
        }
        public override void Write(byte[] dest, uint offset, object value)
        {
            BitConverter<ushort>.GetBytes(dest, value, (int)offset);
        }
    }
    public class UInt32Field : BinaryField
    {
        public override Type DataType { get { return typeof(uint); } }

        public UInt32Field(string name, uint offset) : base(name, offset, 4) { }

        public override object Read(byte[] source, uint offset)
        {
            return BitConverter.ToUInt32(source, (int)offset);
        }
        public override void Write(byte[] dest, uint offset, object value)
        {
            BitConverter<uint>.GetBytes(dest, value, (int)offset);
        }
    }
    public class UInt64Field : BinaryField
    {
        public override Type DataType { get { return typeof(ulong); } }

        public UInt64Field(string name, uint offset) : base(name, offset, 8) { }

        public override object Read(byte[] source, uint offset)
        {
            return BitConverter.ToUInt64(source, (int)offset);
        }
        public override void Write(byte[] dest, uint offset, object value)
        {
            BitConverter<ulong>.GetBytes(dest, value, (int)offset);
        }
    }

    public class SingleField : BinaryField
    {
        public override Type DataType { get { return typeof(float); } }

        public SingleField(string name, uint offset) : base(name, offset, 4) { }

        public override object Read(byte[] source, uint offset)
        {
            return BitConverter.ToSingle(source, (int)offset);
        }
        public override void Write(byte[] dest, uint offset, object value)
        {
            BitConverter<float>.GetBytes(dest, value, (int)offset);
        }
    }
    public class DoubleField : BinaryField
    {
        public override Type DataType { get { return typeof(double); } }

        public DoubleField(string name, uint offset) : base(name, offset, 8) { }

        public override object Read(byte[] source, uint offset)
        {
            return BitConverter.ToDouble(source, (int)offset);
        }
        public override void Write(byte[] dest, uint offset, object value)
        {
            BitConverter<double>.GetBytes(dest, value, (int)offset);
        }
    }

    public class StringField : BinaryField
    {
        public override Type DataType { get { return typeof(string); } }

        public StringField(string name, uint offset, uint length) : base(name, offset, length) { }

        public override object Read(byte[] source, uint offset)
        {
            return Tools.TakeString(source, (int)offset, (int)Length);
        }
        public override void Write(byte[] dest, uint offset, object value)
        {
            Tools.PutBytes(dest, (int)offset, (string)value, (int)Length);
        }

    }

    #endregion


    // Struct is a class that defines a struct format
    // It has data about its size, field names, types and positions
    // And it also has a name
    //
    // It has methods to serialize/deserialize its data and
    // it can export it to a datatable in a single line

    public class Struct
    {
        public string Name { get; }

        public uint Length { get; }

        public IField[] Fields { get; }

        // Initializes a "struct" definition given a name, length (in bytes) and its fields

        public Struct(string name, uint length, params IField[] fields)
        {
            Name = name;
            Length = length;
            Fields = fields;
        }

        // Gets the index of the field that matches this name or returns -1

        public int this[string name]
        {
            get
            {
                for (int i = 0; i < Fields.Length; i++)

                    if (Fields[i].Name == name) return i;

                return -1;
            }
        }

        // Deserializes an instance of this "struct" definition from a stream
        // Returns an array of objects that represent the struct's values

        public virtual object[] Read(Stream source)
        {
            var buffer = new byte[Length];

            source.Read(buffer, 0, (int)Length);

            return Fields.Select(x => x.Read(buffer, x.Offset)).ToArray();
        }

        // Serializes an instance of this "struct" definition to a stream
        // Accepts an array of objects that represent the struct's values

        public virtual void Write(Stream dest, object[] value)
        {
            var buffer = new byte[Length];

            for (var i = 0; i < value.Length; i++)

                if (!Convert.IsDBNull(value[i]))

                    Fields[i].Write(buffer, Fields[i].Offset, value[i]);

            dest.Write(buffer, 0, (int)Length);
        }

        // Deserializes multiple instances ... using Read
        // Returns a collection of the read instances

        public IEnumerable<object[]> ReadMany(Stream source, uint count)
        {
            if (count > source.Length) throw new ArgumentException();

            for (int i = 0; i < count; ++i) yield return Read(source);
        }
        
        // Serializes multiple instances ... using Write
        // Accepts a collection of the instances to write

        public void WriteMany(Stream dest, IEnumerable<object[]> value) => value.Foreach(v => Write(dest, v));
        
        // Returns the schema in a new T : DataTable

        public T GetSchema<T>()
            where T : DataTable, new()
        {
            T dt = new T();

            dt.TableName = Name;

            foreach (IField f in Fields) dt.Columns.Add(f.Name, f.DataType);

            dt.CaseSensitive = false;

            return dt;
        }

        // Same as above but fills the datatable with data

        public T GetSchema<T>(IEnumerable<object[]> data)
            where T : DataTable, new()
        {
            var temp = GetSchema<T>();

            foreach (var row in data) temp.Rows.Add(row as object[]);

            return temp;
        }

        // Default GetSchema<T> type is DataTable

        public DataTable GetSchema() => GetSchema<DataTable>();

        // Default GetSchema<T> type is DataTable

        public DataTable GetSchema(IEnumerable<object[]> data) => GetSchema<DataTable>(data);
    }
}


namespace Formatter.Inf
{
    public static class ObjectInfoFormatter
    {
        static IField[][] formats = new IField[][]
        {
            #region header

            new IField[]
            {
                new StringField("CharacterFileName", 0, 20),
                new SInt32Field("BodyConditionNumber", 20),
            },

            #endregion

            #region BodyConditionData

            new IField[]
            {
                new UInt64Field("BodyCondition", 0),
                new StringField("BodyConditionName", 8, 20),
                new Bool32Field("AlphaBlending", 28),
                new SInt32Field("TextureRenderState", 32),
                new SInt32Field("DestBlend", 36),
                new SInt32Field("SrcBlend", 40),
                new SInt32Field("EffectNum", 44),
                new SingleField("StartAnimTime", 48),
                new SingleField("EndAnimTime", 52),
                new Bool32Field("Rendering", 56),
                new Bool32Field("NotAnimLooping", 60),
                new SingleField("AnimVelocity", 64),
                new StringField("SoundFileName", 68, 20),
            },

            #endregion

            #region effects

            new IField[]
            {
                new UInt32Field("EffectType", 0),
                new StringField("EffectName", 4, 20),

                new SingleField("Pos X", 24),
                new SingleField("Pos Y", 28),
                new SingleField("Pos Z", 32),
                new SingleField("Target X", 36),
                new SingleField("Target Y", 40),
                new SingleField("Target Z", 44),
                new SingleField("Up X", 48),
                new SingleField("Up Y", 52),
                new SingleField("Up Z", 56),

                new SingleField("StartTime", 60),
                new Bool32Field("UseBillboard", 64),

                new SingleField("BillboardAngle", 68),
                new SingleField("BillboardRotateAngle", 72),
                new SingleField("BillboardRotatePerSec", 76),

                new SingleField("RandomUpLargeAngle", 80),
                new SingleField("RandomUpSmallAngle", 84),

                new Bool32Field("UseCharacterMatrix", 88),
                new Bool32Field("GroundBillboard", 92),
            },

            #endregion
        };

        // specialized struct that contains an array of another struct
        // used to read body condition data and the related effects

        class BodyCondFormatter : Struct
        {

            public BodyCondFormatter(string name, uint length, IField[] fields) : base(name, length, fields) { }

            public override object[] Read(Stream source)
            {
                var temp = new object[2];
                temp[0] = base.Read(source);
                temp[1] = formatters[2].ReadMany(source, (uint)(int)(temp[0] as object[])[6]).ToArray();
                return temp;
            }

            public override void Write(Stream dest, object[] value)
            {
                base.Write(dest, value[0] as object[]);
                formatters[2].WriteMany(dest, value[1] as IEnumerable<object[]>);
            }
        }

        static Struct[] formatters = new Struct[]
        {
            new Struct("header", 24, formats[0]),
            new BodyCondFormatter("data", 88, formats[1]),
            new Struct("effects", 96, formats[2]),
        };

        public static IField[] GetFormat(string name) =>
            
            formatters.FirstOrDefault(x => x.Name == name)?.Fields;

        public static Struct GetFormatter(string name) =>
            
            formatters.FirstOrDefault(x => x.Name == name);


        public static object[] Read(Stream source)
        {
            var temp = new object[2];

            temp[0] = formatters[0].Read(source);
            temp[1] = formatters[1].ReadMany(source, (uint)(int)(temp[0] as object[])[1]).ToArray();

            return temp;
        }

        public static void Write(Stream dest, object[] value)
        {
            formatters[0].Write(dest, value[0] as object[]);
            formatters[1].WriteMany(dest, value[1] as IEnumerable<object[]>);
        }

        public static DataTableEx GetSchema(string name) =>
            
            formatters.FirstOrDefault(x => x.Name == name)?.GetSchema<DataTableEx>();

        public static DataTableEx GetSchema(string name, IEnumerable<object[]> value) =>
            
            formatters.FirstOrDefault(x => x.Name == name)?.GetSchema<DataTableEx>(value);

        public static IField[] BodyConditionFormat { get { return formats[1]; } }
        public static IField[] EffectFormat { get { return formats[2]; } }
    }

    public static class EffectInfoFormatter
    {
        private static readonly IField[][] formats = new IField[][]
        {
            new IField[]
            #region ObjectAni
            {
                new StringField("Name", 8 - 8, 20),
                new StringField("ObjectFile", 28 - 8, 20),
                new SingleField("Scale", 48 - 8),
                new SInt32Field("ObjectAniType", 52 - 8),
                new SingleField("TextureAniVel", 56 - 8),
                new SingleField("ObjectAniVel", 60 - 8),
                new SingleField("Tick", 64 - 8),
                new Bool32Field("ZbufferEnable", 68 - 8),

                new SingleField("ColorR", 72 - 8),
                new SingleField("ColorG", 76 - 8),
                new SingleField("ColorB", 80 - 8),
                new SingleField("ColorA", 84 - 8),

                new SingleField("ColorStepR", 88 - 8),
                new SingleField("ColorStepG", 92 - 8),
                new SingleField("ColorStepB", 96 - 8),
                new SingleField("ColorStepA", 100 - 8),

                new SingleField("ColorChangeStartTime", 104 - 8),
                new Bool32Field("AlphaBlending", 108 - 8),

                new SInt32Field("SrcBlend", 112 - 8),
                new SInt32Field("DestBlend", 116 - 8),
                new SInt32Field("TextureRenderState", 120 - 8),
                new Bool32Field("LightMapUse", 124 - 8),
                new Bool32Field("LightMapAlphaBlending", 128 - 8),
                new SInt32Field("LightMapSrcBlend", 132 - 8),
                new SInt32Field("LightMapDestBlend", 136 - 8),
                new SInt32Field("LightMapRenderState", 140 - 8),
                new Bool32Field("AnimationLoop", 144 - 8),
                new SingleField("StartTime", 148 - 8),
                new SingleField("EndTime", 152 - 8),
                new Bool32Field("ObjectAnimationLoop", 156 - 8),
                new SInt32Field("ColorLoop", 160 - 8),
                new Bool32Field("UseEnvironmentLight", 164 - 8),
                new Bool32Field("AlphaTestEnable", 168 - 8),
                new SInt32Field("AlphaTestValue", 172 - 8),
                new Bool32Field("ZWriteEnable", 176 - 8),
            },
            #endregion

            new IField[]
            #region SpriteAni
            {
                new StringField("Name", 8 - 8, 20),

                new SInt32Field("TextureVertexBufferType", 28 - 8),

                new SingleField("TextureSize", 32 - 8),
                new SingleField("Velocity", 36 - 8),
                new SingleField("Tick", 40 - 8),

                new Bool32Field("ZbufferEnable", 44 - 8),

                new SingleField("ColorR", 48 - 8),
                new SingleField("ColorG", 52 - 8),
                new SingleField("ColorB", 56 - 8),
                new SingleField("ColorA", 60 - 8),

                new SingleField("ColorStepR", 64 - 8),
                new SingleField("ColorStepG", 68 - 8),
                new SingleField("ColorStepB", 72 - 8),
                new SingleField("ColorStepA", 76 - 8),

                new SingleField("ColorChangeStartTime", 80 - 8),
                new Bool32Field("AlphaBlending", 84 - 8),

                new StringField("TextureFile", 88 - 8, 20),

                new SInt32Field("SrcBlend", 108 - 8),
                new SInt32Field("DestBlend", 112 - 8),
                new SInt32Field("TextureRenderState", 116 - 8),
                new Bool32Field("LightMapAlphaBlending", 120 - 8),
                new StringField("LightMapFile", 124 - 8, 20),
                new SInt32Field("LightMapSrcBlend", 144 - 8),
                new SInt32Field("LightMapDestBlend", 148 - 8),
                new SInt32Field("LightMapRenderState", 152 - 8),

                new SInt32Field("ColorLoop", 156 - 8),
                new Bool32Field("ZWriteEnable", 160 - 8),
            },
            #endregion

            new IField[]
            #region Particle
            {
                new StringField("Name", 8 - 8, 20),
                new Bool32Field("Loop", 28 - 8),
                new SingleField("DelayTime", 32 - 8),
                new SingleField("CurrentDelayTime", 36 - 8),
                new UInt32Field("DestBlend", 40 - 8),
                new UInt32Field("SrcBlend", 44 - 8),
                new SInt32Field("EmitMass", 48 - 8),
                new SingleField("EmitTime", 52 - 8),
                new SingleField("CurrentEmitTime", 56 - 8),
                new SingleField("Gravity", 60 - 8),
                new SingleField("EmitLifeTime", 64 - 8),
                new SingleField("CurrentEmitLifeTime", 68 - 8),
                new SingleField("ParticleLifeTime", 72 - 8),

                new SingleField("StartColorR", 76 - 8),
                new SingleField("StartColorG", 80 - 8),
                new SingleField("StartColorB", 84 - 8),
                new SingleField("StartColorA", 88 - 8),
                new SingleField("ColorVelR", 92 - 8),
                new SingleField("ColorVelG", 96 - 8),
                new SingleField("ColorVelB", 100 - 8),
                new SingleField("ColorVelA", 104 - 8),

                new SingleField("PositionX", 108 - 8),
                new SingleField("PositionY", 112 - 8),
                new SingleField("PositionZ", 116 - 8),
                new SingleField("VelocityX", 120 - 8),
                new SingleField("VelocityY", 124 - 8),
                new SingleField("VelocityZ", 128 - 8),

                new SingleField("TextureSizeVel", 132 - 8),
                new SingleField("TextureStartSize", 136 - 8),
                new SingleField("TextureSizeMax", 140 - 8),
                new SingleField("TextureSizeMin", 144 - 8),
                new SingleField("Tick", 148 - 8),
                new SInt32Field("TextureSizeChangeType", 152 - 8),
                new SingleField("CurrentTick", 156 - 8),

                new SingleField("DirectionX", 160 - 8),
                new SingleField("DirectionY", 164 - 8),
                new SingleField("DirectionZ", 168 - 8),
                new SingleField("AreaX", 172 - 8),
                new SingleField("AreaY", 176 - 8),
                new SingleField("AreaZ", 180 - 8),

                new Bool32Field("CreateRandom", 184 - 8),
                new SingleField("CircleForce", 188 - 8),
                new SingleField("CreateDensity", 192 - 8),
                new SingleField("EmitRadius", 196 - 8),
                new SingleField("RotateAngle", 200 - 8),
                new SingleField("TextureAnimationTime", 204 - 8),
                new SInt32Field("EmitterType", 208 - 8),
                new SInt32Field("TextureAnimationType", 212 - 8),
                new SInt32Field("ParticleType", 216 - 8),
                new SingleField("EmitAngle", 220 - 8),
                new SInt32Field("TextureNumber", 224 - 8),

                new StringField("TextureName00", 228 - 8, 20),
                new StringField("TextureName01", 248 - 8, 20),
                new StringField("TextureName02", 268 - 8, 20),
                new StringField("TextureName03", 288 - 8, 20),
                new StringField("TextureName04", 308 - 8, 20),
                new StringField("TextureName05", 328 - 8, 20),
                new StringField("TextureName06", 348 - 8, 20),
                new StringField("TextureName07", 368 - 8, 20),
                new StringField("TextureName08", 388 - 8, 20),
                new StringField("TextureName09", 408 - 8, 20),
                new StringField("TextureName10", 428 - 8, 20),
                new StringField("TextureName11", 448 - 8, 20),
                new StringField("TextureName12", 468 - 8, 20),
                new StringField("TextureName13", 488 - 8, 20),
                new StringField("TextureName14", 508 - 8, 20),
                new StringField("TextureName15", 528 - 8, 20),
                new StringField("TextureName16", 548 - 8, 20),
                new StringField("TextureName17", 568 - 8, 20),
                new StringField("TextureName18", 588 - 8, 20),
                new StringField("TextureName19", 608 - 8, 20),

                new SInt32Field("Persistence1", 628 - 8),
                new Bool32Field("ZbufferEnable", 632 - 8),
                new SingleField("ColorChangeStartTime", 636 - 8),
                new SInt32Field("ColorLoop", 640 - 8),

                new SInt32Field("ObjCreateTargetType", 644 - 8),
                new SInt32Field("ObjCreateUpType", 648 - 8),
                new SInt32Field("ObjMoveTargetType", 652 - 8),
                new Bool32Field("ZWriteEnable", 656 - 8),
            },
            #endregion

            new IField[]
            #region Trace
            {
                new UInt32Field("Type", 0),
                new SingleField("Distance", 4),

                new StringField("Name", 8, 20),
                new UInt32Field("NumberOfTrace", 28),
                new SingleField("CreateTick", 32),
                new SingleField("HalfSize", 36),
                new UInt32Field("TextureNumber", 40),
                new SingleField("TextureAnimationTime", 44),


                new StringField("TextureName00", 48, 20),
                new StringField("TextureName01", 68, 20),
                new StringField("TextureName02", 88, 20),
                new StringField("TextureName03", 108, 20),
                new StringField("TextureName04", 128, 20),
                new StringField("TextureName05", 148, 20),
                new StringField("TextureName06", 168, 20),
                new StringField("TextureName07", 188, 20),
                new StringField("TextureName08", 208, 20),
                new StringField("TextureName09", 228, 20),
                new StringField("TextureName10", 248, 20),
                new StringField("TextureName11", 268, 20),
                new StringField("TextureName12", 288, 20),
                new StringField("TextureName13", 308, 20),
                new StringField("TextureName14", 328, 20),
                new StringField("TextureName15", 348, 20),
                new StringField("TextureName16", 368, 20),
                new StringField("TextureName17", 388, 20),
                new StringField("TextureName18", 408, 20),
                new StringField("TextureName19", 428, 20),

                new SInt32Field("NumberOfCross", 448),

                new Bool32Field("AlphaBlendEnable", 452),

                new UInt32Field("SrcBlend", 456),
                new UInt32Field("DestBlend", 460),
                new UInt32Field("TextureRenderState", 464),

                new Bool32Field("ZbufferEnable", 468),
                new Bool32Field("ZWriteEnable", 472),
            },
                #endregion

            new IField[]
            #region Header was left last intentionally
                {
                    new UInt32Field("ProductID", 0),
                    new UInt32Field("Version", 4),
                    new UInt32Field("EffectType", 8),
                },
            #endregion
        };

        private static readonly Struct[] formatters = new Struct[]
        {
            new Struct("ObjectAniData", 172, formats[0]),
            new Struct("SpriteAniData", 158, formats[1]),
            new Struct("ParticleData", 652, formats[2]),
            new Struct("TraceData", 476, formats[3]),
            new Struct("header", 12, formats[4]),
        };
        
        public static Struct HeaderFormatter => formatters[4];

        public static IField[] GetFormat(uint type) => (type < 4) ? formats[type] : null;
        public static Struct GetFormatter(uint type) => (type < 4) ? formatters[type] : null;
        
        public static DataTable GetSchema(uint type) => GetFormatter(type)?.GetSchema();
        public static DataTable GetSchema(uint type, IEnumerable<object[]> data) => GetFormatter(type)?.GetSchema(data);

        public static int GetIndex(uint type, string name) => GetFormatter(type)?[name] ?? -1;


        public static object[] Read(Stream source)
        {
            if (source == null) return null;

            var temp = new object[2];

            temp[0] = HeaderFormatter.Read(source);

            var t = (uint)(temp[0] as object[])[2];

            temp[1] = GetFormatter(t).Read(source);

            return temp;
        }

        public static void Write(Stream dest, object[] value)
        {
            var t = (uint)(value[0] as object[])[2];

            HeaderFormatter.Write(dest, value[0] as object[]);
            GetFormatter(t).Write(dest, value[1] as object[]);
        }
    }
}

namespace Formatter.Omi
{
    // deprecated

    [Obsolete("Use Library.FormatterOmi instead.")]
    public static class OmiFormatter
    {
        private static IField[] ti_item = new IField[]
        #region Initializer
            {
                new Bool32Field("Disabled", 460), // reusing this


                new UInt32Field("ItemNum", 0),
                new UInt8Field("Kind", 4),
                new StringField("ItemName", 5, 40),
                new SingleField("AbilityMin", 48),
                new SingleField("AbilityMax", 52),
                new UInt16Field("ReqRace", 56),
                new UInt16Field("ReqAttackPart", 58),
                new UInt16Field("ReqDefensePart", 60),
                new UInt16Field("ReqFuelPart", 62),
                new UInt16Field("ReqSoulPart", 64),
                new UInt16Field("ReqShieldPart", 66),
                new UInt16Field("ReqDodgePart", 68),
                new UInt16Field("ReqUnitKind", 70),
                new UInt8Field("ReqMinLevel", 72),
                new UInt8Field("ReqMaxLevel", 73),
                new UInt16Field("ReqItemKind", 74),
                new UInt16Field("Weight", 76),
                //filler 2 bytes
                new SingleField("HitRate", 80),
                new SingleField("Defense", 84),
                new SingleField("FractionResistance", 88),
                new UInt8Field("NaturalFaction", 92),
                new UInt8Field("SpeedPenalty", 93),
                new UInt16Field("Range", 94),
                new UInt8Field("Position", 96),
                new UInt8Field("Scarcity", 97),
                //filler 3 bytes
                new SingleField("Endurance", 100),
                new UInt8Field("AbrasionRate", 104),
                new UInt16Field("Charging", 106),
                new UInt16Field("Luck", 108),
                new UInt16Field("MinTradeQuantity", 110),
                new UInt32Field("Price", 112),
                new UInt32Field("CashPrice", 116),
                new UInt16Field("DestParameter1", 120),
                new UInt16Field("DestParameter2", 122),
                new UInt16Field("DestParameter3", 124),
                new UInt16Field("DestParameter4", 126),
                new UInt16Field("DestParameter5", 128),
                new UInt16Field("DestParameter6", 130),
                new UInt16Field("DestParameter7", 132),
                new UInt16Field("DestParameter8", 134),
                new SingleField("ParameterValue1", 136),
                new SingleField("ParameterValue2", 140),
                new SingleField("ParameterValue3", 144),
                new SingleField("ParameterValue4", 148),
                new SingleField("ParameterValue5", 152),
                new SingleField("ParameterValue6", 156),
                new SingleField("ParameterValue7", 160),
                new SingleField("ParameterValue8", 164),
                new UInt32Field("ReAttacktime", 168),
                new UInt32Field("Time", 172),
                new UInt16Field("RepeatTime", 176),
                new UInt16Field("Material", 178),
                new UInt16Field("ReqMaterial", 182),
                new SingleField("RangeAngle", 184),
                new UInt32Field("UpgradeNum", 188),
                new UInt32Field("LinkItem", 192),
                new UInt16Field("MultiTarget", 196),
                new UInt16Field("ExplosionRange", 198),
                new UInt16Field("ReactionRange", 200),
                new UInt8Field("ShotNum", 202),
                new UInt8Field("MultiNum", 203),
                new UInt16Field("AttackTime", 204),
                new UInt16Field("ReqSP", 206),
                new UInt32Field("SummonMonster", 208),
                new UInt32Field("NextSkill", 212),
                new UInt16Field("SkillLevel", 216),
                new UInt16Field("SkillHitRate", 218),
                new UInt8Field("SkillType", 220),
                new UInt8Field("SkillTargetType", 221),
                new UInt8Field("Caliber", 222),
                new UInt8Field("OrbitType", 223),
                new UInt64Field("ItemAttribute", 224),
                new SingleField("BoosterAngle", 232),
                new UInt32Field("CameraPattern", 236),
                new UInt32Field("SourceIndex", 240),
                //filler 4 bytes, there is a pointer here in reality
                new StringField("Description", 248, 200),
                new UInt32Field("EnchantCheckDestParam", 448),
                new UInt32Field("InvokingDestParamID", 452),
                new UInt32Field("InvokingDestParamIDByUse", 456),

                //filler 12 bytes

                /* 	InvokingWearItemDestParamList*	pInvokingDestParamList;
	             *  InvokingWearItemDestParamList*	pInvokingDestParamByUseList;
	             *  BYTE IsTenderDropItem; */
            };
        #endregion

        private static Struct s0 = new Struct("ti_item", 472, ti_item);

        [Obsolete]
        public static IEnumerable<object[]> Read(Stream source)
        {
            var type = source.Read<uint>();
            var count = source.Read<uint>();

            return s0.ReadMany(source, count);
        }

        [Obsolete]
        public static void Write(Stream dest, IEnumerable<object[]> value)
        {
            dest.Write(0);
            dest.Write(value.Count());
            s0.WriteMany(dest, value);
        }

        public static int IndexOf(string name)
        {
            for (int i = 0; i < ti_item.Length; ++i) if (name == ti_item[i].Name) return i;

            throw new IndexOutOfRangeException();
        }

        [Obsolete]
        public static DataTableEx GetSchema()
        {
            var schema = s0.GetSchema<DataTableEx>();

            schema.PrimaryKey = new[] { schema.Columns["ItemNum"] };

            return schema;
        }

        public static DataTableEx GetSchema(IEnumerable<object[]> value)
        {
            var schema = s0.GetSchema<DataTableEx>(value);

            schema.PrimaryKey = new[] { schema.Columns["ItemNum"] };

            return schema;
        }

        [Obsolete]
        public static IField[] ItemFormat { get { return ti_item; } }

        public static Struct item => s0;

        public class ItemRowComparer : IEqualityComparer<object>
        {
            public new bool Equals(object x, object y)
            {
                if (x == null && y == null) return true;

                else if (x == null || y == null) return false;

                else if ((uint)(x as object[])[1] == (uint)(y as object[])[1]) return true;

                else return false;
            }

            public int GetHashCode(object x)
            {
                return ((uint)(x as object[])[1]).GetHashCode();
            }
        }
    }
}

namespace Formatter.Sma
{
    public static class SmaFormatter
    {
        static IField[][] formats = new IField[][]
        {
            #region header
            new IField[]
            {
                new UInt32Field("x00", 0),
                new UInt32Field("x04", 4),
                new UInt32Field("x08", 8),
                new UInt32Field("x0C", 12),
                new UInt32Field("x10", 16),
            },
            #endregion

            #region tileinfo
            new IField[]
            {
                new SingleField("X", 0),
                new SingleField("Y", 4),
                new SingleField("Z", 8),
                new UInt32Field("MoveType", 12),
                new UInt8Field("PKType", 16),
                new Bool32Field("EnableLand", 20),
                new UInt8Field("EventType", 24),
                new UInt16Field("EventParam1", 26),
                new UInt16Field("EventParam2", 28),
                new UInt16Field("EventParam3", 30),
                new StringField("EventStrParam", 32, 40),
                new UInt32Field("WaterType", 72)
            },
            #endregion

            #region monsterinfo
            new IField[]
            {
                new StringField("Region Name", 0, 40),
                new UInt32Field("Monster Num", 40),
                new UInt16Field("Start X", 44),
                new UInt16Field("Start Y", 46),
                new UInt16Field("End X", 48),
                new UInt16Field("End Y", 50),
                new UInt16Field("Max Count", 52),
                new UInt16Field("Respawn Count", 54),
                new UInt16Field("Respawn Time", 56),
                new UInt8Field("Monster Type", 58),
                // filler 1 byte
            }
            #endregion
        };

        static Struct[] formatters = new[]
        {
            new Struct("header", 20, formats[0]),
            new Struct("tileinfo", 76, formats[1]),
            new Struct("monsterinfo", 60, formats[2])
        };
        
        public static object[] Read(Stream source, uint dim)
        {
            // here we can see first return value is a Struct and the next two are Table

            var temp = new object[3];

            temp[0] = formatters[0].Read(source).ToArray();

            temp[1] = formatters[1].ReadMany(source, dim * dim).ToArray();

            var c = source.Read<uint>();

            temp[2] = formatters[2].ReadMany(source, c).ToArray();

            return temp;

        }
        public static void Write(Stream dest, object[] value)
        {
            formatters[0].Write(dest, value[0] as object[]);

            formatters[1].WriteMany(dest, value[1] as IEnumerable<object[]>);

            dest.Write<int>((value[2] as IEnumerable<object[]>).Count());

            formatters[2].WriteMany(dest, value[2] as IEnumerable<object[]>);

        }
        
        // Gets the schema you requested or null
        public static DataTable GetSchema(string name)
        {
            foreach (var formatter in formatters) if (name == formatter.Name) return formatter.GetSchema();

            return null;
        }

        public static int GetIndex(string schema, string field)
        {
            foreach (var formatter in formatters) if (schema == formatter.Name) return formatter[field];

            return -1;
        }

        public static Struct GetFormatter(int type) => formatters[type];
    }
}

namespace Formatter.Dat
{
    public static class MapFormatter
    {
        static IField[][] formats = new IField[][]
        {
            #region Initializer

            new IField[]
            {
                new SingleField("TileSize", 0), //float
                new SInt32Field("VertexNumber", 4), //int
                new SInt32Field("TileInfoNumber", 8),//int 
                new SInt32Field("ObjectNumber", 12),//int
            },

            new IField[]
            {
                new SingleField("Pos X", 0),
                new SingleField("Pos Y", 4),
                new SingleField("Pos Z", 8),
                new SingleField("Normal X", 12),
                new SingleField("Normal Y", 16),
                new SingleField("Normal Z", 20),
            },

            new IField[]
            {
                new SInt32Field("TexNumber", 0), //int
                new UInt32Field("WaterType", 4), //bool     by this one ? and type is difrent so difrent collors ? i think this one is like "enable/disable water"
                new SInt32Field("WaterHeight", 8),//int
                new SInt16Field("WaterTexNum", 12),//short      // i think client doesn't know whats in right ;) so if i use this to paint crap whit that funkcion u made i think we can kinda draw it already lets try ? go
                new UInt8Field("Move", 14),//byte
                new SInt32Field("EnableLand", 16),//bool
                new UInt8Field("EventType", 20),//byte
            },

            new IField[]
            {
                new UInt32Field("ObjType", 0),
                new SInt32Field("ObjCount", 4),
                new SingleField("Pos X", 8),
                new SingleField("Pos Y", 12),
                new SingleField("Pos Z", 16),

                new SingleField("Vel X", 20),
                new SingleField("Vel Y", 24),
                new SingleField("Vel Z", 28),

                new SingleField("Up X", 32),
                new SingleField("Up Y", 36),
                new SingleField("Up Z", 40),

                new UInt32Field("ObjectMonsterUniqueNumber", 44),
                new UInt8Field("ObjectTexIndex", 48),
                new UInt32Field("ObjectSrcIndex", 52),
                new UInt8Field("BodyCondition", 56),
                new SInt32Field("IsEvent", 60),
                new UInt8Field("EventType", 64),

                new SInt16Field("EventIndexFrom", 66),
                new SInt16Field("EventIndexTo", 68),
                new SInt16Field("EventIndex3", 70),

                new StringField("EventParam", 72, 40),
            },

            #endregion
        };

        static Struct[] formatters = new[]
        {
            new Struct("mapdata", 16, formats[0]),
            new Struct("vertexinfo", 24, formats[1]),
            new Struct("tileinfo", 24, formats[2]),
            new Struct("objectinfo", 112, formats[3]),
        };

        public static object[] Read(Stream source)
        {
            var temp = new object[4];
            source.Position +=20;        
            temp[0] = formatters[0].Read(source).ToArray();

            // what these lines here tell us is that Read returns an array of 3 tables

            temp[1] = formatters[1].ReadMany(source, (uint)(int)(temp[0] as object[])[1]).ToArray();
            temp[2] = formatters[2].ReadMany(source, (uint)(int)(temp[0] as object[])[2]).ToArray();
            temp[3] = formatters[3].ReadMany(source, (uint)(int)(temp[0] as object[])[3]).ToArray();

            return temp;
        }


        // Gets the schema you requested or null
        public static DataTable GetSchema(string name)
        {
            foreach (var formatter in formatters) if (name == formatter.Name) return formatter.GetSchema();

            return null;
        }

        public static int GetIndex(string schema, string field)
        {
            foreach (var formatter in formatters) if (schema == formatter.Name) return formatter[field];

            return -1;
        }

        public static Struct GetFormatter(int type) => formatters[type];
    }
}

namespace Formatter.Omd
{
    public static class OmdFormatter
    {
        static IField[][] formats = new IField[][]
        {
            #region Initializer

            new IField[]
            {
                new SingleField("m_EncodeNum", 0), //float
                new SInt32Field("m_DataSize", 4), //int
                new SInt32Field("m_Parity", 8),//int 
                new StringField("FileName",12,10),// string
                new SInt32Field("m_pData", 22),//char
            }

      
            #endregion
        };

        static Struct[] formatters = new[]
        {
            new Struct("omddata", 26, formats[0]),
          
        };

        public static object[] Read(Stream source)
        {
            var temp = new object[4];
            source.Position += 20;
            temp[0] = formatters[0].Read(source).ToArray();

            return temp;
        }


        // Gets the schema you requested or null
        public static DataTable GetSchema(string name)
        {
            foreach (var formatter in formatters) if (name == formatter.Name) return formatter.GetSchema();

            return null;
        }

        public static int GetIndex(string schema, string field)
        {
            foreach (var formatter in formatters) if (schema == formatter.Name) return formatter[field];

            return -1;
        }

        public static Struct GetFormatter(int type) => formatters[type];
    }
}