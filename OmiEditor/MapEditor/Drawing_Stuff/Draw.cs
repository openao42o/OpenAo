using System;
using System.Collections.Generic;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;


class Draw
{

    private static Panel MainContainer = new Panel();
    public static IEnumerable<Tuple<string, int, Zbox, int>> MonsterData = null;
   

    private static Bitmap GetTexture()
    {
        return null;
    }

    public static void DrawMonsters()
    {
    
        

        foreach(var spawn in MonsterData)
        {
           // Zbox temp = new Zbox(spawn.Item3);
        }


    }

    public static IEnumerable<Tuple<string, int, Zbox, int>> PrepareMonsterData(object[][] data)
    {
     
        for (int i = 0; i < data.Length; i++)
        {
            yield return new Tuple<string, int, Zbox, int>(
                //region name
                data[i][0]?.ToString(),
                //monsternum                                                  
                int.Parse(data[i][1]?.ToString()),
                //new Rectangle for drawing later on                                               
                new Zbox(new Rectangle(
                    int.Parse(data[i][2]?.ToString()) * 2,
                    int.Parse(data[i][3]?.ToString()) * 2,
                    int.Parse(data[i][4]?.ToString()) * 2 - int.Parse(data[i][2]?.ToString()) * 2,
                    int.Parse(data[i][5]?.ToString()) * 2 - int.Parse(data[i][3]?.ToString()) * 2)),
                //max monster count                                                              
                int.Parse(data[i][6]?.ToString()));
        }

    }





}

