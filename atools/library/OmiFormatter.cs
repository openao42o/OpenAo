using Formatter;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Library
{

    public static class FormatterOmi
    {
        public class RowComparer<T> : IEqualityComparer<object[]>
            where T : IEquatable<T>
        {
            uint index { get; }

            public RowComparer(uint index) { this.index = index; }

            public bool Equals(object[] x, object[] y)
            {
                if (x == null && y == null) return true;

                else if (x == null || y == null) return false;

                else return ((T)(x as object[])[index]).Equals((T)(y as object[])[index]);
            }

            public int GetHashCode(object[] x)
            {
                return ((T)(x as object[])[index]).GetHashCode();
            }
        }

        public class RowComparer<T1, T2> : IEqualityComparer<object[]>
            where T1 : IEquatable<T1>
            where T2 : IEquatable<T2>
        {
            uint index1 { get; }
            uint index2 { get; }

            public RowComparer(uint idx1, uint idx2)
            {
                if (idx1 == idx2) throw new ArgumentException("idx1 cannot be equal to idx2");

                index1 = idx1;
                index2 = idx2;
            }

            public bool Equals(object[] x, object[] y)
            {
                if (x == null && y == null) return true;

                else if (x == null || y == null) return false;

                else return
                        ((T1)(x as object[])[index1]).Equals((T1)(y as object[])[index1])
                        &&
                        ((T2)(x as object[])[index2]).Equals((T2)(y as object[])[index2]);
            }

            public int GetHashCode(object[] x)
            {
                return
                    (((T1)(x as object[])[index1]).GetHashCode() +
                    ((long)((T1)(x as object[])[index1]).GetHashCode() << 32)).GetHashCode();
            }
        }

        public class RowComparer<T1, T2, T3> : IEqualityComparer<object[]>
            where T1 : IEquatable<T1>
            where T2 : IEquatable<T2>
            where T3 : IEquatable<T3>
        {
            uint[] indeces { get; }

            public RowComparer(params uint[] indeces)
            {
                if (indeces.Any(x => indeces.Count(y => y == x) != 1))

                    throw new ArgumentException("Indeces must be distinct");

                this.indeces = indeces;
            }

            public bool Equals(object[] x, object[] y)
            {
                if (x == null && y == null) return true;

                else if (x == null || y == null) return false;

                var x0 = x as object[];
                var y0 = y as object[];

                return
                        ((T1)x0[indeces[0]]).Equals((T1)y0[indeces[0]]) &&
                        ((T2)x0[indeces[1]]).Equals((T2)y0[indeces[1]]) &&
                        ((T3)x0[indeces[2]]).Equals((T3)y0[indeces[2]]);
            }

            public int GetHashCode(object[] x)
            {
                var x0 = x as object[];

                var hash0 = ((T1)x0[indeces[0]]).GetHashCode();
                var hash1 = ((T2)x0[indeces[1]]).GetHashCode();
                var hash2 = ((T3)x0[indeces[2]]).GetHashCode();

                return ((((hash2 * 31) + hash1) * 23) + hash0) * 17;
            }
        }

        public class RowComparer<T1, T2, T3, T4, T5, T6> : IEqualityComparer<object[]>
            where T1 : IEquatable<T1>
            where T2 : IEquatable<T2>
            where T3 : IEquatable<T3>
            where T4 : IEquatable<T4>
            where T5 : IEquatable<T5>
            where T6 : IEquatable<T6>
        {
            uint[] indeces { get; }

            public RowComparer(params uint[] indeces)
            {
                if (indeces.Any(x => indeces.Count(y => y == x) != 1))

                    throw new ArgumentException("Indeces must be distinct");

                this.indeces = indeces;
            }

            public bool Equals(object[] x, object[] y)
            {
                if (x == null && y == null) return true;

                else if (x == null || y == null) return false;

                var x0 = x as object[];
                var y0 = y as object[];

                return
                        ((T1)x0[indeces[0]]).Equals((T1)y0[indeces[0]]) &&
                        ((T2)x0[indeces[1]]).Equals((T2)y0[indeces[1]]) &&
                        ((T3)x0[indeces[2]]).Equals((T3)y0[indeces[2]]) &&
                        ((T4)x0[indeces[3]]).Equals((T4)y0[indeces[3]]) &&
                        ((T5)x0[indeces[4]]).Equals((T5)y0[indeces[4]]) &&
                        ((T6)x0[indeces[5]]).Equals((T6)y0[indeces[5]]);
            }

            public int GetHashCode(object[] x)
            {
                var x0 = x as object[];

                var hash0 = ((T1)x0[indeces[0]]).GetHashCode();
                var hash1 = ((T2)x0[indeces[1]]).GetHashCode();
                var hash2 = ((T3)x0[indeces[2]]).GetHashCode();
                var hash3 = ((T4)x0[indeces[3]]).GetHashCode();
                var hash4 = ((T5)x0[indeces[4]]).GetHashCode();
                var hash5 = ((T6)x0[indeces[5]]).GetHashCode();

                return ((((((((((hash5 * 13) + hash4) * 17) + hash3) * 23) + hash2) * 31) + hash1) * 23) + hash0) * 17;
            }
        }

        public class MixingRowComparer : IEqualityComparer<object[]>
        {
            uint targetidx { get; }

            uint[] sourceidx { get; }

            public MixingRowComparer(uint targetidx, params uint[] sourceidx)
            {

                if (sourceidx.Any(x => sourceidx.Count(y => y == x) != 1 || x == targetidx))

                    throw new ArgumentException("Indeces must be distinct");

                this.targetidx = targetidx;
                this.sourceidx = sourceidx;
            }

            public bool Equals(object[] x, object[] y)
            {
                if (x == null && y == null) return true;

                else if (x == null || y == null) return false;

                var x0 = x as object[];
                var y0 = y as object[];

                var srcx = x0.Where((obj, i) => sourceidx.Contains((uint)i)).Select(obj => (uint)obj).OrderBy(num => num);
                var srcy = y0.Where((obj, i) => sourceidx.Contains((uint)i)).Select(obj => (uint)obj).OrderBy(num => num);

                return (uint)x0[targetidx] == (uint)y0[targetidx] && srcx.SequenceEqual(srcy);
            }

            public int GetHashCode(object[] x)
            {
                var x0 = x as object[];

                ulong hash = (ulong)(uint)x0[targetidx] * 2147483647;

                foreach (var num in x0.Where((obj, i) => sourceidx.Contains((uint)i)).Select(obj => (uint)obj))

                    if (num != 0) hash += num * num;

                return hash.GetHashCode();
            }
        }


        #region Omi Formats

        static IField[][] formats = new IField[][]
        {
            #region ti_item

            new IField[]
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
            },

            #endregion

            #region ti_monster

            new IField[]
            {
                new UInt32Field("UniqueNumber", 0),
                new StringField("MonsterName", 4, 40),
                new UInt8Field("Level", 44),
                new UInt32Field("HP", 48),
                new UInt8Field("Size", 52),  //dont wonder about this
                new UInt8Field("Belligerence", 53),
                new UInt8Field("AlphaBlending", 54),//1:true 0:false
                new UInt16Field("RenderIndex", 56),
                new SingleField("ScaleValue", 60),//float
                new UInt8Field("TextureIndex", 64),
                new UInt32Field("SourceIndex", 68),
                new UInt64Field("MPOption", 72), //BitFlag64_t     
                new UInt8Field("ClickEvent", 80),
                new StringField("PortraitFileName", 84, 50)
            },

            #endregion

            #region ti_mapobject

            new IField[]
            {
                new UInt32Field("Code", 0),
                new UInt8Field("Alpha", 4),
                new UInt8Field("Collision", 5),
                new UInt8Field("CollisionForServer", 6),
                new UInt8Field("ObjectRenderType", 7),
                new UInt16Field("RenderIndex", 8)
            },

            #endregion

            #region ti_rareiteminfo

            new IField[]
            {
                new UInt32Field("CodeNum", 0),
                new StringField("Name", 4, 30),
                new UInt32Field("ReqUseType", 36),
                new UInt32Field("ReqMinLevel", 40),
                new UInt32Field("ReqMaxLevel", 44),
                new UInt8Field("ReqItemKind", 48),//byte
                new UInt16Field("ReqAttackPart", 50),
                new UInt16Field("ReqDefensePart", 52),
                new UInt16Field("ReqDodgePart", 54),
                new UInt16Field("ReqFuelPart", 56),
                new UInt16Field("ReqShieldPart", 58),
                new UInt16Field("ReqSoulPart", 60),
                new UInt8Field("DesParameter1", 62),
                new UInt8Field("DesParameter2", 63),
                new UInt8Field("DesParameter3", 64),
                new UInt8Field("DesParameter4", 65),
                new UInt8Field("DesParameter5", 66),
                new UInt8Field("DesParameter6", 67),
                new UInt8Field("DesParameter7", 68),
                new UInt8Field("DesParameter8", 69),
                new UInt8Field("DesParameter9", 70),
                new SingleField("ParameterValue1", 72),
                new SingleField("ParameterValue2", 76),
                new SingleField("ParameterValue3", 80),
                new SingleField("ParameterValue4", 84),
                new SingleField("ParameterValue5", 88),
                new SingleField("ParameterValue6", 92),
                new SingleField("ParameterValue7", 96),
                new SingleField("ParameterValue8", 100),
                new SingleField("ParameterValue9", 104),
                new SInt32Field("Probability100k", 108)

            },

            #endregion

            #region ti_building

            new IField[]
            {
                new UInt32Field("BuildingIndex", 0),
                new StringField("BuildingName", 4, 40),
                new UInt8Field("BuildingKind", 44),//byte!
                new UInt16Field("MapIndex", 46),
                new UInt32Field("NPCIndex", 48),
                new StringField("NPCName", 52, 40),
                new StringField("GreetingTalk", 92, 1000),
                new UInt32Field("OwnerCityMapIndex", 1092),
                new UInt32Field("OwnerOrderNumber", 1096)
            },

            #endregion

            #region ti_mapinfo

            new IField[]
            {
                new UInt32Field("MapIndex", 0),
                new UInt16Field("BeforeMapIndex", 4),
                new StringField("MapName", 6, 40),
                new UInt16Field("MapInfluenceType", 46),
                new UInt32Field("CityMapIndex", 48),
                new UInt32Field("CityWarQuestIndex", 52),
                new UInt32Field("CityWarQuestNPCIndex", 56),
                new UInt32Field("CityWarQuestMonsterUID", 60),
                new UInt16Field("MaxUserCount", 64),
                new UInt16Field("MaxGroundHeight", 66),
                new UInt16Field("MinGroundHeight", 68),
                new UInt16Field("WaterHeight", 70),
                new UInt16Field("UserVisibleDistance", 72),
                new UInt16Field("MonsterVisibleDistance", 74),
                new UInt32Field("QuestIndexForWarp", 76),
                new UInt16Field("FrontPositionDistance", 80),
                new UInt16Field("AutoRecoveryFlag", 82),
                new UInt16Field("DefaultMapIndex", 84),
                new UInt16Field("CityWarpTargetPositionX", 86),
                new UInt16Field("CityWarpTargetPositionY", 88),
                new UInt16Field("CityWarpTargetPositionZ", 90),
                new UInt32Field("ClientMaxAltitudeHeight", 92),
                new UInt32Field("DayFogColor", 96),
                new UInt32Field("NightFogColor", 100),
                new UInt16Field("DayFogStartDistance", 104),
                new UInt16Field("DayFogEndDistance", 106),
                new UInt16Field("NightFogStartDistance", 108),
                new UInt16Field("NightFogEndDistance", 110),
                new UInt16Field("DaySunDirectionX", 112),
                new UInt16Field("DaySunDirectionY", 114),
                new UInt16Field("DaySunDirectionZ", 116),
                new UInt16Field("NightSunDirectionX", 118),
                new UInt16Field("NightSunDirectionY", 120),
                new UInt16Field("NightSunDirectionZ", 122),
                new UInt8Field("WaterType", 124),
                new UInt8Field("PKZoneFlag", 125),
                new UInt8Field("TileRenderingFlag", 126),
                new UInt8Field("SkyRenderingFlag", 127),
                new UInt8Field("SunRenderingFlag", 128),
                new UInt16Field("FogRenderingFlag", 129),
                //filler 1 byte
                new UInt32Field("VCNWarpObjectIndex", 132),
                new UInt32Field("ANIWarpObjectIndex", 136),
                new UInt16Field("dat", 140),
                new UInt16Field("map", 142),
                new UInt16Field("tex", 144),
                new UInt16Field("cloud", 146),
                new UInt16Field("sky", 148),
                new UInt16Field("nsky", 150),
                new UInt32Field("bgm", 152),
                new UInt32Field("TeleportWarpObjectIndex", 156),
                new UInt16Field("BeforeMapIndex2", 160),
                new StringField("Description", 162, 1024)
            },

            #endregion

            #region ti_mixing

            new IField[]
            {
                new UInt32Field("TargetItemNum", 0),
                new UInt16Field("MixingProbability", 4),
                new UInt32Field("MixingCost", 8),
                new UInt32Field("SourceItemNum1", 12),
                new UInt32Field("SourceItemCount1", 16),
                new UInt32Field("SourceItemNum2", 20),
                new UInt32Field("SourceItemCount2", 24),
                new UInt32Field("SourceItemNum3", 28),
                new UInt32Field("SourceItemCount3", 32),
                new UInt32Field("SourceItemNum4", 36),
                new UInt32Field("SourceItemCount4", 40),
                new UInt32Field("SourceItemNum5", 44),
                new UInt32Field("SourceItemCount5", 48),
                new UInt32Field("NumOfSourceItems",52),
                new UInt8Field("Visible", 56)
            },

            #endregion

            #region ti_mysteryitem

            new IField[]
            {
                new UInt32Field("MysteryItemDropNum", 0),
                new UInt16Field("ReqUnitKind", 4),
                new UInt8Field("ReqMinLevel", 6),
                new UInt8Field("ReqMaxLevel", 7),
                new UInt32Field("DropItemNum", 8),
                new UInt32Field("MinCount", 12),
                new UInt32Field("MaxCount", 16),
                new UInt32Field("Probability", 20),
                new UInt32Field("PrefixProbability", 24),
                new UInt32Field("SuffixProbability", 28),
                new UInt16Field("Period", 32),
                new UInt32Field("CountPerPeriod", 36),
                new UInt32Field("DropCount", 40), 
                //start time
                new UInt16Field("Year", 44),
                new UInt8Field("Month", 46),
                new UInt8Field("Day", 47),
                new UInt8Field("Hour", 48),
                new UInt8Field("Minute", 49),
                new UInt8Field("Second", 50)
                //end of time
            },

            #endregion

            #region ti_luckymachine

            new IField[]//done
            {
                new UInt32Field("MachineOrder", 0),
                new UInt32Field("BuildingIndex", 4),
                new UInt8Field("MachineKind", 8),
                new UInt32Field("MachineNum", 12),
                new UInt32Field("CoinItemNum", 16),
                new UInt8Field("SlotNum", 20),
                new UInt32Field("SourceIndex", 24),
                new StringField("Description", 28, 200)
            },

            #endregion

            #region ti_invoking

            new IField[]
            {
                new UInt32Field("InvokingDestParamID", 0),
                new UInt16Field("InvokingDestParam", 4),
                new SingleField("InvokingDestParamValue", 8),
                new UInt32Field("InvokingEffectIdx", 12)
            },

            new IField[]
            {
                new UInt32Field("InvokingDestParamIDByUse", 0),
                new UInt16Field("InvokingDestParamByUse", 4),
                new SingleField("InvokingDestParamValueByUse", 8),
                new UInt32Field("InvokingEffectIdxByUse", 12)
            },

            #endregion

            #region ti_burningmap

            new IField[]
            {
                new UInt32Field("RestrictedMapIndex", 0),
                new UInt16Field("MapIndex", 4),
                new UInt16Field("ReqUnitKind", 6),
                new UInt8Field("MinLevel", 8),
                new UInt8Field("MaxLevel", 9)
            },

            #endregion
            
            #region ti_pet

            new IField[]
            {
                new SInt32Field("PetIndex", 0),
                new StringField("PetName", 4, 40),
                new SInt32Field("PetKind", 44),
                new Bool32Field("EnableName", 48),
                new Bool32Field("EnableLevel", 52),
                new SInt32Field("MaxLevel", 56),
                new SInt32Field("BaseSocketCount", 60),
            },

            new IField[]
            {
                new SInt32Field("UniqueNumber", 0),
                new SInt32Field("PetLeveL", 4),
                new DoubleField("NeedExp", 8),
                new SInt32Field("UseWeaponIndex", 16),
                new UInt8Field("SlotCountSkill", 20),
                new UInt8Field("SlotCountSocket", 21),
                new UInt8Field("KitLevelHP", 22),
                new UInt8Field("KitLevelShield", 23),
                new UInt8Field("KitLevelSP", 24)
            },

            #endregion

            #region ti_operatoraction

            new IField[]
            {
                new SInt32Field("ActionType", 0),
                new SInt32Field("ItemNum", 4),
                new SInt32Field("FunctionIndex", 8),
                new DoubleField("FunctionValue", 16),
                new SInt32Field("SourceIndex", 24),
                new StringField("ActionDesc", 28, 128)
            },

            #endregion

            #region ti_dissolution

            new IField[]
            {
                new UInt32Field("InputItemNum",0),
                new UInt32Field("OutputItemNum1", 4),
                new UInt32Field("OutputItemNum2", 8),
                new UInt32Field("OutputItemNum3", 12),
                new UInt32Field("MinCount1", 16),
                new UInt32Field("MinCount2", 20),
                new UInt32Field("MinCount3", 24),
                new UInt32Field("MaxCount1", 28),
                new UInt32Field("MaxCount2", 32),
                new UInt32Field("MaxCount3", 36),
                new UInt32Field("Probability1", 40),
                new UInt32Field("Probability2", 44),
                new UInt32Field("Probability3", 48),
            },

            #endregion

            #region ti_monstermultitarget

            new IField[]
            {
                new UInt32Field("MonsterUniqueNumber", 0),
                new UInt32Field("PointIndex", 4),
                new SingleField("X", 8),
                new SingleField("Y", 12),
                new SingleField("Z", 16)

            },

            #endregion
        };

        #endregion

        #region Row Equality Comparers

        static IEqualityComparer<object[]>[] rowcomparers = new IEqualityComparer<object[]>[]
        {
            new RowComparer<uint>(1), // ti_item
            new RowComparer<uint>(0), // ti_monster
            new RowComparer<uint>(0), // ti_mapobject
            new RowComparer<uint>(0), // ti_rareiteminfo
            new RowComparer<uint, ushort>(0, 3), // ti_building
            new RowComparer<uint>(0), // ti_mapinfog
            new MixingRowComparer(0, 3, 5, 7, 9, 11), // ti_mixing
            new RowComparer<uint, uint>(0, 4), // ti_mysteryitem
            new RowComparer<uint, uint, byte, uint, uint, byte>(0, 1, 2, 3, 4, 5), // ti_luckymachine
            new RowComparer<uint, ushort>(0, 1), // ti_invoking
            new RowComparer<uint, ushort>(0, 1), // ti_invoking
            new RowComparer<uint>(0), // ti_burningmap
            new RowComparer<int>(0), // ti_pet
            new RowComparer<int, int>(0, 1), // ti_petlevel
            new RowComparer<int, int, int>(0, 1, 2), // ti_operatoraction
            new RowComparer<uint>(0), // ti_dissolution
            new RowComparer<uint, uint>(0, 1), // ti_multitarget
        };

        #endregion

        #region Formatters

        static Struct[] formatters = new Struct[]
        {
            new Struct("ti_Item", 472, formats[0]),
            new Struct("ti_Monster", 136, formats[1]),
            new Struct("ti_MapObject", 12, formats[2]),
            new Struct("ti_RareItemInfo", 112, formats[3]),
            new Struct("ti_Building", 1100, formats[4]),
            new Struct("ti_MapInfo", 1188, formats[5]),
            new Struct("ti_ItemMixingInfo", 60, formats[6]),
            new Struct("ti_MysteryItemDrop", 52, formats[7]),
            new Struct("ti_LuckyMachine", 228, formats[8]),
            new Struct("ti_InvokingWearItemDPNum", 16, formats[9]),
            new Struct("ti_InvokingWearItemDPNumByUse", 16, formats[10]),
            new Struct("ti_BurningMap", 12, formats[11]),
            new Struct("ti_Pet", 64, formats[12]),
            new Struct("ti_PetLevel", 32, formats[13]),
            new Struct("ti_OperatorAction", 160, formats[14]),
            new Struct("ti_DissolutionItem", 52, formats[15]),
            new Struct("ti_MonsterMultiTarget", 20, formats[16]),
        };

        static Struct[] formattersAO = new Struct[]
        {
            new Struct("ti_Item", 576, formats[0]),
            new Struct("ti_Monster", 136, formats[1]),
            new Struct("ti_MapObject", 12, formats[2]),
            new Struct("ti_RareItemInfo", 112, formats[3]),
            new Struct("ti_Building", 1100, formats[4]),
            new Struct("ti_MapInfo", 1188, formats[5]),
            new Struct("ti_ItemMixingInfo", 60, formats[6]),
            new Struct("ti_MysteryItemDrop", 52, formats[7]),
            new Struct("ti_LuckyMachine", 228, formats[8]),
            new Struct("ti_InvokingWearItemDPNum", 16, formats[9]),
            new Struct("ti_InvokingWearItemDPNumByUse", 16, formats[10]),
            new Struct("ti_BurningMap", 12, formats[11]),
            new Struct("ti_Pet", 64, formats[12]),
            new Struct("ti_PetLevel", 32, formats[13]),
            new Struct("ti_OperatorAction", 160, formats[14]),
            new Struct("ti_DissolutionItem", 52, formats[15]),
            new Struct("ti_MonsterMultiTarget", 20, formats[16]),
        };

        #endregion


        public static Struct[] Formatters => formatters;
        public static IEqualityComparer<object[]>[] RowComparers => rowcomparers;

        public static object[] Read(Stream source)
        {
            var temp = new object[formatters.Length];

            for (int i = 0; i < formatters.Length; ++i)
            {
                var type = source.Read<uint>();
                var count = source.Read<uint>();
                temp[i] = formatters[i].ReadMany(source, count).ToArray();
            }

            return temp;
        }

        public static object[] ReadAO(Stream source)
        {
            var temp = new object[formatters.Length];

            for (int i = 0; i < formatters.Length; ++i)
            {
                var type = source.Read<uint>();
                var count = source.Read<uint>();
                temp[i] = formattersAO[i].ReadMany(source, count).ToArray();
            }

            return temp;
        }

        public static void Write(Stream dest, object[] value)
        {
            Func<int, int> getcode = x => x > 4 ? x + 1 : x; // if x > 4 then return x + 1 else x => because in omi they skip index 5

            for (int i = 0; i < formatters.Length; ++i)
            {
                dest.Write(getcode(i));
                dest.Write((value[i] as object[]).Length);
                formatters[i].WriteMany(dest, value[i] as object[][]);
            }
        }


        public static Struct GetFormatter(string name) => formatters.FirstOrDefault(x => x.Name == name);


        public static DataTableEx GetSchema(string name) => GetFormatter(name)?.GetSchema<DataTableEx>();

        public static DataTableEx GetSchema(string name, IEnumerable<object[]> value) => GetFormatter(name)?.GetSchema<DataTableEx>(value);
        

    }
    
}