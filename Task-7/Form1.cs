using System;
using System.Collections.Generic;
using System.Drawing;
using System.Windows.Forms;

namespace BuildingSimulation
{
    public partial class Form1 : Form
    {
        private System.Windows.Forms.Timer timer;
        private Random random;
        private List<Building> buildings;

        public Form1()
        {
            InitializeComponent();
            timer = new System.Windows.Forms.Timer();
            timer.Interval = 30; // Update every 30 ms
            timer.Tick += Timer_Tick;
            random = new Random();
            buildings = new List<Building>();

            this.Paint += Form1_Paint;
            this.DoubleBuffered = true;
        }

        private void Timer_Tick(object sender, EventArgs e)
        {
            foreach (var building in buildings)
            {
                building.Move();
            }
            Invalidate();
        }

        private void Form1_Paint(object sender, PaintEventArgs e)
        {
            Graphics g = e.Graphics;
            foreach (var building in buildings)
            {
                building.Draw(g);
            }

            // Draw the boundary lines for the simulation areas
            Pen pen = new Pen(Color.Black, 1);
            int halfWidth = ClientSize.Width / 2;
            int halfHeight = ClientSize.Height / 2;

            // Draw top-left quarter boundary
            g.DrawRectangle(pen, 0, 0, halfWidth, halfHeight);
            // Draw bottom-right quarter boundary
            g.DrawRectangle(pen, halfWidth, halfHeight, halfWidth, halfHeight);
        }

        private void StartSimulationButton_Click(object sender, EventArgs e)
        {
            for (int i = 0; i < 10; i++) // Generate some buildings
            {
                buildings.Add(new CapitalBuilding(random, ClientSize.Width, ClientSize.Height));
                buildings.Add(new WoodenBuilding(random, ClientSize.Width, ClientSize.Height));
            }
            timer.Start();
        }

        private void StopSimulationButton_Click(object sender, EventArgs e)
        {
            timer.Stop();
        }

        private void ClearButton_Click(object sender, EventArgs e)
        {
            buildings.Clear();
            Invalidate();
        }
    }

    public abstract class Building
    {
        protected Point position;
        protected int speed;
        protected Random random;
        protected int width, height;

        public Building(Random random, int width, int height)
        {
            this.random = random;
            this.width = width;
            this.height = height;
            this.speed = random.Next(1, 5);
        }

        public abstract void Move();
        public abstract void Draw(Graphics g);
    }

    public class CapitalBuilding : Building
    {
        public CapitalBuilding(Random random, int width, int height) : base(random, width, height)
        {
            position = new Point(random.Next(width / 2), random.Next(height / 2));
        }

        public override void Move()
        {
            if (position.X > 0 && position.Y > 0)
            {
                position.X -= speed;
                position.Y -= speed;
            }
        }

        public override void Draw(Graphics g)
        {
            g.FillRectangle(Brushes.Gray, position.X, position.Y, 10, 10);
        }
    }

    public class WoodenBuilding : Building
    {
        public WoodenBuilding(Random random, int width, int height) : base(random, width, height)
        {
            position = new Point(random.Next(width / 2, width), random.Next(height / 2, height));
        }

        public override void Move()
        {
            if (position.X < width && position.Y < height)
            {
                position.X += speed;
                position.Y += speed;
            }
        }

        public override void Draw(Graphics g)
        {
            g.FillRectangle(Brushes.Brown, position.X, position.Y, 10, 10);
        }
    }
}
