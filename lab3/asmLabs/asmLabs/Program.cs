using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

namespace asmLabs
{
    class Program
    {
        [DllImport("lab6assembler.dll", EntryPoint = "start")]
        private static extern int lab6();
        [DllImport("lab7assembler.dll", EntryPoint = "DllWindows")]
        private static extern int lab7();
        static void Main(string[] args)
        {
            bool exit = false;
            while (!exit)
            {
                Console.WriteLine("Выберите лабораторную работу(6/7)");
                string c = Console.ReadLine();
                switch (c[0])
                {
                    case '6':
                        lab6();
                        break;
                    case '7':
                        lab7();
                        break;
                }
            }
        }
    }
}
