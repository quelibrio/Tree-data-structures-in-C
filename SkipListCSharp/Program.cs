using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using DataStructures;

namespace Test
{
    class Program
    {
        static void Main(string[] args)
        {
            Random rand = new Random(1000);
            SkipList skipList = new SkipList();
            for (int i = 0; i < 10000; i++)
            {
                skipList.Add(rand.Next(), 1);
            }

            skipList.Add(9999,5);
            if (skipList.Contains(9999))
            {
                Console.WriteLine(skipList[9999]);
            }
        }
    }
}
