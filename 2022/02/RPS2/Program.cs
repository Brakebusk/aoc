using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading;

namespace RPS2
{
    internal class Program
    {
        static void Main(string[] args)
        {
            int THREADCOUNT = 16;
            List<int> opponent = new List<int>();
            List<int> outcomes = new List<int>();
            int[] totals = new int[THREADCOUNT];
            Barrier barrier = new Barrier(THREADCOUNT + 1);

            foreach (string line in System.IO.File.ReadLines("input.txt"))
            {
                string[] lineValues = line.Split(' ');
                opponent.Add(Convert.ToInt16(lineValues[0][0]) - 65);
                outcomes.Add(Convert.ToInt16(lineValues[1][0]) - 88);
            }


            Thread[] workers = new Thread[THREADCOUNT];
            int sliceSize = opponent.Count() / THREADCOUNT;
            for (int i = 0; i < THREADCOUNT; i++)
            {
                int sliceStart = i * sliceSize;
                int sliceEnd = sliceStart + sliceSize;
                if (i == THREADCOUNT - 1) sliceEnd = outcomes.Count();

                Worker worker = new Worker(i, totals, sliceStart, sliceEnd, opponent, outcomes, barrier);
                Thread thread = new Thread(new ThreadStart(worker.Run));
                workers[i] = thread;
                thread.Start();
            }

            barrier.SignalAndWait();
            Console.WriteLine("Part 2: {0}, ({1})", totals.Sum(), string.Join(", ", totals));
        }

        private class Worker
        {
            int id;
            int[] totals;
            int sliceStart;
            int sliceEnd;
            List<int> opponent;
            List<int> outcomes;
            Barrier barrier;

            int[,] scoreMatrix = new int[3, 3] {
                {3, 4, 8},
                {1, 5, 9},
                {2, 6, 7},
            };

            public Worker(int id, int[] totals, int sliceStart, int sliceEnd, List<int> opponent, List<int> outcomes, Barrier barrier)
            {
                this.id = id;
                this.totals = totals;
                this.sliceStart = sliceStart;
                this.sliceEnd = sliceEnd;
                this.opponent = opponent;
                this.outcomes = outcomes;
                this.barrier = barrier;
            }
            public void Run()
            {
                for (int i = sliceStart; i < sliceEnd; i++)
                {
                    totals[id] += scoreMatrix[opponent[i], outcomes[i]];
                }
                barrier.SignalAndWait();
            }
        }
    }
}
