namespace RapidCheck
{
    partial class Form1
    {
        /// <summary>
        /// 필수 디자이너 변수입니다.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// 사용 중인 모든 리소스를 정리합니다.
        /// </summary>
        /// <param name="disposing">관리되는 리소스를 삭제해야 하면 true이고, 그렇지 않으면 false입니다.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form 디자이너에서 생성한 코드

        /// <summary>
        /// 디자이너 지원에 필요한 메서드입니다.
        /// 이 메서드의 내용을 코드 편집기로 수정하지 마십시오.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Form1));
            this.materialTabControl1 = new MaterialSkin.Controls.MaterialTabControl();
            this.tabPage2 = new System.Windows.Forms.TabPage();
            this.panelObject = new System.Windows.Forms.Panel();
            this.menuStrip1 = new System.Windows.Forms.MenuStrip();
            this.시간출력ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.onToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.offToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.조건초기화ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.방향설정ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.toolStripMenuItem2 = new System.Windows.Forms.ToolStripMenuItem();
            this.아래ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.영진짱ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.color0ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.color1ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.color2ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.color3ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.color4ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.color5ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.color6ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.color7ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.color8ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.color9ToolStripMenuItem = new System.Windows.Forms.ToolStripMenuItem();
            this.dataGridView1 = new System.Windows.Forms.DataGridView();
            this.Column1 = new System.Windows.Forms.DataGridViewImageColumn();
            this.dataGridView2 = new System.Windows.Forms.DataGridView();
            this.Column2 = new System.Windows.Forms.DataGridViewImageColumn();
            this.panelProgress = new System.Windows.Forms.Panel();
            this.VideoBtn = new System.Windows.Forms.Button();
            this.progressBar1 = new System.Windows.Forms.ProgressBar();
            this.panelVideo = new System.Windows.Forms.Panel();
            this.startBtn = new MaterialSkin.Controls.MaterialFlatButton();
            this.radioButtonX4 = new System.Windows.Forms.RadioButton();
            this.radioButtonX2 = new System.Windows.Forms.RadioButton();
            this.radioButtonX1 = new System.Windows.Forms.RadioButton();
            this.panelVideoPart = new System.Windows.Forms.Panel();
            this.pictureBoxVideo = new System.Windows.Forms.PictureBox();
            this.trackBar1 = new System.Windows.Forms.TrackBar();
            this.tabPage3 = new System.Windows.Forms.TabPage();
            this.videoPanel = new System.Windows.Forms.Panel();
            this.axWindowsMediaPlayer1 = new AxWMPLib.AxWindowsMediaPlayer();
            this.tabPage1 = new System.Windows.Forms.TabPage();
            this.pictureBox1 = new System.Windows.Forms.PictureBox();
            this.tabPage4 = new System.Windows.Forms.TabPage();
            this.panelWebbrowser = new System.Windows.Forms.Panel();
            this.webBrowser1 = new System.Windows.Forms.WebBrowser();
            this.materialTabSelector1 = new MaterialSkin.Controls.MaterialTabSelector();
            this.openFileDialog1 = new System.Windows.Forms.OpenFileDialog();
            this.timer1 = new System.Windows.Forms.Timer(this.components);
            this.pictureBoxLogo = new System.Windows.Forms.PictureBox();
            this.imageList1 = new System.Windows.Forms.ImageList(this.components);
            this.materialTabControl1.SuspendLayout();
            this.tabPage2.SuspendLayout();
            this.panelObject.SuspendLayout();
            this.menuStrip1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView1)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView2)).BeginInit();
            this.panelProgress.SuspendLayout();
            this.panelVideo.SuspendLayout();
            this.panelVideoPart.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBoxVideo)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.trackBar1)).BeginInit();
            this.tabPage3.SuspendLayout();
            this.videoPanel.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.axWindowsMediaPlayer1)).BeginInit();
            this.tabPage1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
            this.tabPage4.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBoxLogo)).BeginInit();
            this.SuspendLayout();
            // 
            // materialTabControl1
            // 
            this.materialTabControl1.Controls.Add(this.tabPage2);
            this.materialTabControl1.Controls.Add(this.tabPage3);
            this.materialTabControl1.Controls.Add(this.tabPage1);
            this.materialTabControl1.Controls.Add(this.tabPage4);
            this.materialTabControl1.Depth = 0;
            this.materialTabControl1.Dock = System.Windows.Forms.DockStyle.Bottom;
            this.materialTabControl1.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F);
            this.materialTabControl1.Location = new System.Drawing.Point(0, 65);
            this.materialTabControl1.MouseState = MaterialSkin.MouseState.HOVER;
            this.materialTabControl1.Name = "materialTabControl1";
            this.materialTabControl1.SelectedIndex = 0;
            this.materialTabControl1.Size = new System.Drawing.Size(1920, 1015);
            this.materialTabControl1.TabIndex = 0;
            this.materialTabControl1.SelectedIndexChanged += new System.EventHandler(this.materialTabControl1_SelectedIndexChanged);
            // 
            // tabPage2
            // 
            this.tabPage2.Controls.Add(this.panelObject);
            this.tabPage2.Controls.Add(this.panelProgress);
            this.tabPage2.Controls.Add(this.panelVideo);
            this.tabPage2.Location = new System.Drawing.Point(4, 26);
            this.tabPage2.Name = "tabPage2";
            this.tabPage2.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage2.Size = new System.Drawing.Size(1912, 985);
            this.tabPage2.TabIndex = 1;
            this.tabPage2.Text = "tabPage2";
            this.tabPage2.UseVisualStyleBackColor = true;
            // 
            // panelObject
            // 
            this.panelObject.BackColor = System.Drawing.Color.White;
            this.panelObject.Controls.Add(this.menuStrip1);
            this.panelObject.Controls.Add(this.dataGridView1);
            this.panelObject.Controls.Add(this.dataGridView2);
            this.panelObject.Dock = System.Windows.Forms.DockStyle.Top;
            this.panelObject.Location = new System.Drawing.Point(1442, 67);
            this.panelObject.Name = "panelObject";
            this.panelObject.Size = new System.Drawing.Size(467, 923);
            this.panelObject.TabIndex = 2;
            // 
            // menuStrip1
            // 
            this.menuStrip1.Dock = System.Windows.Forms.DockStyle.None;
            this.menuStrip1.GripStyle = System.Windows.Forms.ToolStripGripStyle.Visible;
            this.menuStrip1.ImageScalingSize = new System.Drawing.Size(20, 20);
            this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.시간출력ToolStripMenuItem,
            this.조건초기화ToolStripMenuItem,
            this.방향설정ToolStripMenuItem,
            this.영진짱ToolStripMenuItem});
            this.menuStrip1.Location = new System.Drawing.Point(5, 3);
            this.menuStrip1.Name = "menuStrip1";
            this.menuStrip1.Size = new System.Drawing.Size(371, 28);
            this.menuStrip1.TabIndex = 19;
            this.menuStrip1.Text = "menuStrip1";
            // 
            // 시간출력ToolStripMenuItem
            // 
            this.시간출력ToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.onToolStripMenuItem,
            this.offToolStripMenuItem});
            this.시간출력ToolStripMenuItem.Name = "시간출력ToolStripMenuItem";
            this.시간출력ToolStripMenuItem.Size = new System.Drawing.Size(86, 24);
            this.시간출력ToolStripMenuItem.Text = "시간 출력";
            // 
            // onToolStripMenuItem
            // 
            this.onToolStripMenuItem.Name = "onToolStripMenuItem";
            this.onToolStripMenuItem.Size = new System.Drawing.Size(106, 26);
            this.onToolStripMenuItem.Text = "On";
            this.onToolStripMenuItem.Click += new System.EventHandler(this.onToolStripMenuItem_Click);
            // 
            // offToolStripMenuItem
            // 
            this.offToolStripMenuItem.Name = "offToolStripMenuItem";
            this.offToolStripMenuItem.Size = new System.Drawing.Size(106, 26);
            this.offToolStripMenuItem.Text = "Off";
            this.offToolStripMenuItem.Click += new System.EventHandler(this.offToolStripMenuItem_Click);
            // 
            // 조건초기화ToolStripMenuItem
            // 
            this.조건초기화ToolStripMenuItem.Name = "조건초기화ToolStripMenuItem";
            this.조건초기화ToolStripMenuItem.Size = new System.Drawing.Size(101, 24);
            this.조건초기화ToolStripMenuItem.Text = "조건 초기화";
            this.조건초기화ToolStripMenuItem.Click += new System.EventHandler(this.ResetToolStripMenuItem_Click);
            // 
            // 방향설정ToolStripMenuItem
            // 
            this.방향설정ToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripMenuItem2,
            this.아래ToolStripMenuItem});
            this.방향설정ToolStripMenuItem.Name = "방향설정ToolStripMenuItem";
            this.방향설정ToolStripMenuItem.Size = new System.Drawing.Size(86, 24);
            this.방향설정ToolStripMenuItem.Text = "방향 설정";
            // 
            // toolStripMenuItem2
            // 
            this.toolStripMenuItem2.Name = "toolStripMenuItem2";
            this.toolStripMenuItem2.Size = new System.Drawing.Size(98, 26);
            this.toolStripMenuItem2.Text = "↑";
            this.toolStripMenuItem2.Click += new System.EventHandler(this.UpToolStripMenuItem2_Click);
            // 
            // 아래ToolStripMenuItem
            // 
            this.아래ToolStripMenuItem.Name = "아래ToolStripMenuItem";
            this.아래ToolStripMenuItem.Size = new System.Drawing.Size(98, 26);
            this.아래ToolStripMenuItem.Text = "↓";
            this.아래ToolStripMenuItem.Click += new System.EventHandler(this.DownToolStripMenuItem_Click);
            // 
            // 영진짱ToolStripMenuItem
            // 
            this.영진짱ToolStripMenuItem.DropDownItems.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.color0ToolStripMenuItem,
            this.color1ToolStripMenuItem,
            this.color2ToolStripMenuItem,
            this.color3ToolStripMenuItem,
            this.color4ToolStripMenuItem,
            this.color5ToolStripMenuItem,
            this.color6ToolStripMenuItem,
            this.color7ToolStripMenuItem,
            this.color8ToolStripMenuItem,
            this.color9ToolStripMenuItem});
            this.영진짱ToolStripMenuItem.Name = "영진짱ToolStripMenuItem";
            this.영진짱ToolStripMenuItem.Size = new System.Drawing.Size(86, 24);
            this.영진짱ToolStripMenuItem.Text = "색상 설정";
            // 
            // color0ToolStripMenuItem
            // 
            this.color0ToolStripMenuItem.Name = "color0ToolStripMenuItem";
            this.color0ToolStripMenuItem.Size = new System.Drawing.Size(126, 26);
            this.color0ToolStripMenuItem.Text = "color0";
            this.color0ToolStripMenuItem.Click += new System.EventHandler(this.color0ToolStripMenuItem_Click);
            // 
            // color1ToolStripMenuItem
            // 
            this.color1ToolStripMenuItem.Name = "color1ToolStripMenuItem";
            this.color1ToolStripMenuItem.Size = new System.Drawing.Size(126, 26);
            this.color1ToolStripMenuItem.Text = "color1";
            this.color1ToolStripMenuItem.Click += new System.EventHandler(this.color1ToolStripMenuItem_Click);
            // 
            // color2ToolStripMenuItem
            // 
            this.color2ToolStripMenuItem.Name = "color2ToolStripMenuItem";
            this.color2ToolStripMenuItem.Size = new System.Drawing.Size(126, 26);
            this.color2ToolStripMenuItem.Text = "color2";
            this.color2ToolStripMenuItem.Click += new System.EventHandler(this.color2ToolStripMenuItem_Click);
            // 
            // color3ToolStripMenuItem
            // 
            this.color3ToolStripMenuItem.Name = "color3ToolStripMenuItem";
            this.color3ToolStripMenuItem.Size = new System.Drawing.Size(126, 26);
            this.color3ToolStripMenuItem.Text = "color3";
            this.color3ToolStripMenuItem.Click += new System.EventHandler(this.color3ToolStripMenuItem_Click);
            // 
            // color4ToolStripMenuItem
            // 
            this.color4ToolStripMenuItem.Name = "color4ToolStripMenuItem";
            this.color4ToolStripMenuItem.Size = new System.Drawing.Size(126, 26);
            this.color4ToolStripMenuItem.Text = "color4";
            this.color4ToolStripMenuItem.Click += new System.EventHandler(this.color4ToolStripMenuItem_Click);
            // 
            // color5ToolStripMenuItem
            // 
            this.color5ToolStripMenuItem.Name = "color5ToolStripMenuItem";
            this.color5ToolStripMenuItem.Size = new System.Drawing.Size(126, 26);
            this.color5ToolStripMenuItem.Text = "color5";
            this.color5ToolStripMenuItem.Click += new System.EventHandler(this.color5ToolStripMenuItem_Click);
            // 
            // color6ToolStripMenuItem
            // 
            this.color6ToolStripMenuItem.Name = "color6ToolStripMenuItem";
            this.color6ToolStripMenuItem.Size = new System.Drawing.Size(126, 26);
            this.color6ToolStripMenuItem.Text = "color6";
            this.color6ToolStripMenuItem.Click += new System.EventHandler(this.color6ToolStripMenuItem_Click);
            // 
            // color7ToolStripMenuItem
            // 
            this.color7ToolStripMenuItem.Name = "color7ToolStripMenuItem";
            this.color7ToolStripMenuItem.Size = new System.Drawing.Size(126, 26);
            this.color7ToolStripMenuItem.Text = "color7";
            this.color7ToolStripMenuItem.Click += new System.EventHandler(this.color7ToolStripMenuItem_Click);
            // 
            // color8ToolStripMenuItem
            // 
            this.color8ToolStripMenuItem.Name = "color8ToolStripMenuItem";
            this.color8ToolStripMenuItem.Size = new System.Drawing.Size(126, 26);
            this.color8ToolStripMenuItem.Text = "color8";
            this.color8ToolStripMenuItem.Click += new System.EventHandler(this.color8ToolStripMenuItem_Click);
            // 
            // color9ToolStripMenuItem
            // 
            this.color9ToolStripMenuItem.Name = "color9ToolStripMenuItem";
            this.color9ToolStripMenuItem.Size = new System.Drawing.Size(126, 26);
            this.color9ToolStripMenuItem.Text = "color9";
            this.color9ToolStripMenuItem.Click += new System.EventHandler(this.color9ToolStripMenuItem_Click);
            // 
            // dataGridView1
            // 
            this.dataGridView1.AllowUserToAddRows = false;
            this.dataGridView1.AllowUserToDeleteRows = false;
            this.dataGridView1.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.dataGridView1.Columns.AddRange(new System.Windows.Forms.DataGridViewColumn[] {
            this.Column1});
            this.dataGridView1.Location = new System.Drawing.Point(191, 34);
            this.dataGridView1.Name = "dataGridView1";
            this.dataGridView1.ReadOnly = true;
            this.dataGridView1.RowHeadersVisible = false;
            this.dataGridView1.RowTemplate.Height = 30;
            this.dataGridView1.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.dataGridView1.Size = new System.Drawing.Size(276, 884);
            this.dataGridView1.TabIndex = 20;
            this.dataGridView1.CellClick += new System.Windows.Forms.DataGridViewCellEventHandler(this.dataGridView1_CellClick);
            // 
            // Column1
            // 
            this.Column1.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.Fill;
            this.Column1.HeaderText = "Car";
            this.Column1.ImageLayout = System.Windows.Forms.DataGridViewImageCellLayout.Zoom;
            this.Column1.Name = "Column1";
            this.Column1.ReadOnly = true;
            // 
            // dataGridView2
            // 
            this.dataGridView2.AllowUserToAddRows = false;
            this.dataGridView2.AllowUserToDeleteRows = false;
            this.dataGridView2.ColumnHeadersHeightSizeMode = System.Windows.Forms.DataGridViewColumnHeadersHeightSizeMode.AutoSize;
            this.dataGridView2.Columns.AddRange(new System.Windows.Forms.DataGridViewColumn[] {
            this.Column2});
            this.dataGridView2.Location = new System.Drawing.Point(3, 34);
            this.dataGridView2.Name = "dataGridView2";
            this.dataGridView2.ReadOnly = true;
            this.dataGridView2.RowHeadersVisible = false;
            this.dataGridView2.RowTemplate.Height = 23;
            this.dataGridView2.ScrollBars = System.Windows.Forms.ScrollBars.Vertical;
            this.dataGridView2.Size = new System.Drawing.Size(184, 884);
            this.dataGridView2.TabIndex = 21;
            this.dataGridView2.CellContentClick += new System.Windows.Forms.DataGridViewCellEventHandler(this.dataGridView2_CellContentClick);
            // 
            // Column2
            // 
            this.Column2.AutoSizeMode = System.Windows.Forms.DataGridViewAutoSizeColumnMode.Fill;
            this.Column2.HeaderText = "People";
            this.Column2.MinimumWidth = 50;
            this.Column2.Name = "Column2";
            this.Column2.ReadOnly = true;
            // 
            // panelProgress
            // 
            this.panelProgress.BackColor = System.Drawing.Color.Transparent;
            this.panelProgress.Controls.Add(this.VideoBtn);
            this.panelProgress.Controls.Add(this.progressBar1);
            this.panelProgress.Dock = System.Windows.Forms.DockStyle.Top;
            this.panelProgress.Location = new System.Drawing.Point(1442, 3);
            this.panelProgress.Name = "panelProgress";
            this.panelProgress.Size = new System.Drawing.Size(467, 64);
            this.panelProgress.TabIndex = 1;
            // 
            // VideoBtn
            // 
            this.VideoBtn.Font = new System.Drawing.Font("Microsoft Sans Serif", 13.8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.VideoBtn.Location = new System.Drawing.Point(6, 15);
            this.VideoBtn.Name = "VideoBtn";
            this.VideoBtn.Size = new System.Drawing.Size(71, 34);
            this.VideoBtn.TabIndex = 16;
            this.VideoBtn.Text = "File";
            this.VideoBtn.UseVisualStyleBackColor = true;
            this.VideoBtn.Click += new System.EventHandler(this.VideoBtn_Click);
            // 
            // progressBar1
            // 
            this.progressBar1.BackColor = System.Drawing.SystemColors.ActiveCaption;
            this.progressBar1.Location = new System.Drawing.Point(89, 15);
            this.progressBar1.Name = "progressBar1";
            this.progressBar1.Size = new System.Drawing.Size(357, 34);
            this.progressBar1.TabIndex = 12;
            // 
            // panelVideo
            // 
            this.panelVideo.BackColor = System.Drawing.Color.White;
            this.panelVideo.Controls.Add(this.startBtn);
            this.panelVideo.Controls.Add(this.radioButtonX4);
            this.panelVideo.Controls.Add(this.radioButtonX2);
            this.panelVideo.Controls.Add(this.radioButtonX1);
            this.panelVideo.Controls.Add(this.panelVideoPart);
            this.panelVideo.Controls.Add(this.trackBar1);
            this.panelVideo.Dock = System.Windows.Forms.DockStyle.Left;
            this.panelVideo.Location = new System.Drawing.Point(3, 3);
            this.panelVideo.Name = "panelVideo";
            this.panelVideo.Size = new System.Drawing.Size(1439, 979);
            this.panelVideo.TabIndex = 0;
            // 
            // startBtn
            // 
            this.startBtn.AutoSize = true;
            this.startBtn.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
            this.startBtn.Depth = 0;
            this.startBtn.Location = new System.Drawing.Point(658, 949);
            this.startBtn.Margin = new System.Windows.Forms.Padding(4, 6, 4, 6);
            this.startBtn.MouseState = MaterialSkin.MouseState.HOVER;
            this.startBtn.Name = "startBtn";
            this.startBtn.Primary = false;
            this.startBtn.Size = new System.Drawing.Size(66, 36);
            this.startBtn.TabIndex = 6;
            this.startBtn.Text = "Start";
            this.startBtn.UseVisualStyleBackColor = true;
            // 
            // radioButtonX4
            // 
            this.radioButtonX4.AutoSize = true;
            this.radioButtonX4.Location = new System.Drawing.Point(1172, 952);
            this.radioButtonX4.Name = "radioButtonX4";
            this.radioButtonX4.Size = new System.Drawing.Size(43, 21);
            this.radioButtonX4.TabIndex = 5;
            this.radioButtonX4.TabStop = true;
            this.radioButtonX4.Text = "x4";
            this.radioButtonX4.UseVisualStyleBackColor = true;
            this.radioButtonX4.CheckedChanged += new System.EventHandler(this.radioButtonX4_CheckedChanged);
            // 
            // radioButtonX2
            // 
            this.radioButtonX2.AutoSize = true;
            this.radioButtonX2.Location = new System.Drawing.Point(1123, 952);
            this.radioButtonX2.Name = "radioButtonX2";
            this.radioButtonX2.Size = new System.Drawing.Size(43, 21);
            this.radioButtonX2.TabIndex = 5;
            this.radioButtonX2.TabStop = true;
            this.radioButtonX2.Text = "x2";
            this.radioButtonX2.UseVisualStyleBackColor = true;
            this.radioButtonX2.CheckedChanged += new System.EventHandler(this.radioButtonX2_CheckedChanged);
            // 
            // radioButtonX1
            // 
            this.radioButtonX1.AutoSize = true;
            this.radioButtonX1.Location = new System.Drawing.Point(1074, 952);
            this.radioButtonX1.Name = "radioButtonX1";
            this.radioButtonX1.Size = new System.Drawing.Size(43, 21);
            this.radioButtonX1.TabIndex = 5;
            this.radioButtonX1.TabStop = true;
            this.radioButtonX1.Text = "x1";
            this.radioButtonX1.UseVisualStyleBackColor = true;
            this.radioButtonX1.CheckedChanged += new System.EventHandler(this.radioButtonX1_CheckedChanged);
            // 
            // panelVideoPart
            // 
            this.panelVideoPart.Controls.Add(this.pictureBoxVideo);
            this.panelVideoPart.Dock = System.Windows.Forms.DockStyle.Top;
            this.panelVideoPart.Location = new System.Drawing.Point(0, 0);
            this.panelVideoPart.Name = "panelVideoPart";
            this.panelVideoPart.Size = new System.Drawing.Size(1439, 922);
            this.panelVideoPart.TabIndex = 3;
            // 
            // pictureBoxVideo
            // 
            this.pictureBoxVideo.BackColor = System.Drawing.Color.Transparent;
            this.pictureBoxVideo.Location = new System.Drawing.Point(6, 3);
            this.pictureBoxVideo.Name = "pictureBoxVideo";
            this.pictureBoxVideo.Size = new System.Drawing.Size(1427, 919);
            this.pictureBoxVideo.TabIndex = 1;
            this.pictureBoxVideo.TabStop = false;
            this.pictureBoxVideo.MouseDown += new System.Windows.Forms.MouseEventHandler(this.pictureBoxVideo_MouseDown);
            // 
            // trackBar1
            // 
            this.trackBar1.BackColor = System.Drawing.Color.White;
            this.trackBar1.Enabled = false;
            this.trackBar1.Location = new System.Drawing.Point(158, 928);
            this.trackBar1.Name = "trackBar1";
            this.trackBar1.Size = new System.Drawing.Size(1069, 56);
            this.trackBar1.TabIndex = 2;
            this.trackBar1.TickStyle = System.Windows.Forms.TickStyle.None;
            this.trackBar1.ValueChanged += new System.EventHandler(this.trackBar1_ValueChanged);
            this.trackBar1.MouseDown += new System.Windows.Forms.MouseEventHandler(this.trackBar1_MouseDown);
            // 
            // tabPage3
            // 
            this.tabPage3.Controls.Add(this.videoPanel);
            this.tabPage3.Location = new System.Drawing.Point(4, 26);
            this.tabPage3.Name = "tabPage3";
            this.tabPage3.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage3.Size = new System.Drawing.Size(1912, 985);
            this.tabPage3.TabIndex = 2;
            this.tabPage3.Text = "tabPage3";
            this.tabPage3.UseVisualStyleBackColor = true;
            // 
            // videoPanel
            // 
            this.videoPanel.Controls.Add(this.axWindowsMediaPlayer1);
            this.videoPanel.Dock = System.Windows.Forms.DockStyle.Fill;
            this.videoPanel.Location = new System.Drawing.Point(3, 3);
            this.videoPanel.Name = "videoPanel";
            this.videoPanel.Size = new System.Drawing.Size(1906, 979);
            this.videoPanel.TabIndex = 1;
            // 
            // axWindowsMediaPlayer1
            // 
            this.axWindowsMediaPlayer1.Enabled = true;
            this.axWindowsMediaPlayer1.Location = new System.Drawing.Point(3, 0);
            this.axWindowsMediaPlayer1.Name = "axWindowsMediaPlayer1";
            this.axWindowsMediaPlayer1.OcxState = ((System.Windows.Forms.AxHost.State)(resources.GetObject("axWindowsMediaPlayer1.OcxState")));
            this.axWindowsMediaPlayer1.Size = new System.Drawing.Size(1876, 982);
            this.axWindowsMediaPlayer1.TabIndex = 0;
            // 
            // tabPage1
            // 
            this.tabPage1.Controls.Add(this.pictureBox1);
            this.tabPage1.Location = new System.Drawing.Point(4, 26);
            this.tabPage1.Name = "tabPage1";
            this.tabPage1.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage1.Size = new System.Drawing.Size(1912, 985);
            this.tabPage1.TabIndex = 0;
            this.tabPage1.Text = "tabPage1";
            this.tabPage1.UseVisualStyleBackColor = true;
            // 
            // pictureBox1
            // 
            this.pictureBox1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.pictureBox1.Location = new System.Drawing.Point(3, 3);
            this.pictureBox1.Name = "pictureBox1";
            this.pictureBox1.Size = new System.Drawing.Size(1906, 979);
            this.pictureBox1.SizeMode = System.Windows.Forms.PictureBoxSizeMode.AutoSize;
            this.pictureBox1.TabIndex = 0;
            this.pictureBox1.TabStop = false;
            // 
            // tabPage4
            // 
            this.tabPage4.Controls.Add(this.panelWebbrowser);
            this.tabPage4.Controls.Add(this.webBrowser1);
            this.tabPage4.Location = new System.Drawing.Point(4, 26);
            this.tabPage4.Name = "tabPage4";
            this.tabPage4.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage4.Size = new System.Drawing.Size(1912, 985);
            this.tabPage4.TabIndex = 3;
            this.tabPage4.Text = "tabPage4";
            this.tabPage4.UseVisualStyleBackColor = true;
            // 
            // panelWebbrowser
            // 
            this.panelWebbrowser.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panelWebbrowser.Location = new System.Drawing.Point(3, 3);
            this.panelWebbrowser.Name = "panelWebbrowser";
            this.panelWebbrowser.Size = new System.Drawing.Size(1906, 979);
            this.panelWebbrowser.TabIndex = 1;
            // 
            // webBrowser1
            // 
            this.webBrowser1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.webBrowser1.Location = new System.Drawing.Point(3, 3);
            this.webBrowser1.MinimumSize = new System.Drawing.Size(20, 20);
            this.webBrowser1.Name = "webBrowser1";
            this.webBrowser1.Size = new System.Drawing.Size(1906, 979);
            this.webBrowser1.TabIndex = 0;
            // 
            // materialTabSelector1
            // 
            this.materialTabSelector1.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.materialTabSelector1.BaseTabControl = this.materialTabControl1;
            this.materialTabSelector1.Depth = 0;
            this.materialTabSelector1.Location = new System.Drawing.Point(274, 30);
            this.materialTabSelector1.MouseState = MaterialSkin.MouseState.HOVER;
            this.materialTabSelector1.Name = "materialTabSelector1";
            this.materialTabSelector1.Size = new System.Drawing.Size(1646, 31);
            this.materialTabSelector1.TabIndex = 1;
            this.materialTabSelector1.Text = "materialTabSelector1";
            // 
            // openFileDialog1
            // 
            this.openFileDialog1.FileName = "openFileDialog1";
            // 
            // pictureBoxLogo
            // 
            this.pictureBoxLogo.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.pictureBoxLogo.BackColor = System.Drawing.Color.Transparent;
            this.pictureBoxLogo.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Stretch;
            this.pictureBoxLogo.Image = ((System.Drawing.Image)(resources.GetObject("pictureBoxLogo.Image")));
            this.pictureBoxLogo.Location = new System.Drawing.Point(16, 2);
            this.pictureBoxLogo.Name = "pictureBoxLogo";
            this.pictureBoxLogo.Size = new System.Drawing.Size(203, 57);
            this.pictureBoxLogo.SizeMode = System.Windows.Forms.PictureBoxSizeMode.Zoom;
            this.pictureBoxLogo.TabIndex = 2;
            this.pictureBoxLogo.TabStop = false;
            // 
            // imageList1
            // 
            this.imageList1.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("imageList1.ImageStream")));
            this.imageList1.TransparentColor = System.Drawing.Color.Transparent;
            this.imageList1.Images.SetKeyName(0, "base.png");
            this.imageList1.Images.SetKeyName(1, "base2.png");
            // 
            // Form1
            // 
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.None;
            this.BackgroundImage = ((System.Drawing.Image)(resources.GetObject("$this.BackgroundImage")));
            this.ClientSize = new System.Drawing.Size(1920, 1080);
            this.Controls.Add(this.pictureBoxLogo);
            this.Controls.Add(this.materialTabSelector1);
            this.Controls.Add(this.materialTabControl1);
            this.Font = new System.Drawing.Font("Segoe UI Symbol", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.MainMenuStrip = this.menuStrip1;
            this.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.Name = "Form1";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Load += new System.EventHandler(this.Form1_Load);
            this.materialTabControl1.ResumeLayout(false);
            this.tabPage2.ResumeLayout(false);
            this.panelObject.ResumeLayout(false);
            this.panelObject.PerformLayout();
            this.menuStrip1.ResumeLayout(false);
            this.menuStrip1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView1)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.dataGridView2)).EndInit();
            this.panelProgress.ResumeLayout(false);
            this.panelVideo.ResumeLayout(false);
            this.panelVideo.PerformLayout();
            this.panelVideoPart.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.pictureBoxVideo)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.trackBar1)).EndInit();
            this.tabPage3.ResumeLayout(false);
            this.videoPanel.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.axWindowsMediaPlayer1)).EndInit();
            this.tabPage1.ResumeLayout(false);
            this.tabPage1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
            this.tabPage4.ResumeLayout(false);
            ((System.ComponentModel.ISupportInitialize)(this.pictureBoxLogo)).EndInit();
            this.ResumeLayout(false);

        }

        #endregion

        private MaterialSkin.Controls.MaterialTabControl materialTabControl1;
        private System.Windows.Forms.TabPage tabPage1;
        private System.Windows.Forms.TabPage tabPage2;
        private MaterialSkin.Controls.MaterialTabSelector materialTabSelector1;
        private System.Windows.Forms.TabPage tabPage3;
        private System.Windows.Forms.Panel videoPanel;
        private System.Windows.Forms.OpenFileDialog openFileDialog1;
        private System.Windows.Forms.PictureBox pictureBox1;
        private System.Windows.Forms.Timer timer1;
        private System.Windows.Forms.Panel panelObject;
        private System.Windows.Forms.Panel panelProgress;
        private System.Windows.Forms.Panel panelVideo;
        private System.Windows.Forms.PictureBox pictureBoxVideo;
        private System.Windows.Forms.TrackBar trackBar1;
        private System.Windows.Forms.Panel panelVideoPart;
        private System.Windows.Forms.Button VideoBtn;
        private System.Windows.Forms.ProgressBar progressBar1;
        private AxWMPLib.AxWindowsMediaPlayer axWindowsMediaPlayer1;
        private System.Windows.Forms.RadioButton radioButtonX1;
        private System.Windows.Forms.RadioButton radioButtonX4;
        private System.Windows.Forms.RadioButton radioButtonX2;
        private System.Windows.Forms.MenuStrip menuStrip1;
        private System.Windows.Forms.ToolStripMenuItem 시간출력ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem onToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem offToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem 방향설정ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem 아래ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem toolStripMenuItem2;
        private System.Windows.Forms.ToolStripMenuItem 조건초기화ToolStripMenuItem;
        private System.Windows.Forms.TabPage tabPage4;
        private System.Windows.Forms.WebBrowser webBrowser1;
        private System.Windows.Forms.Panel panelWebbrowser;
        private System.Windows.Forms.PictureBox pictureBoxLogo;
        private System.Windows.Forms.ToolStripMenuItem 영진짱ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem color0ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem color1ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem color2ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem color3ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem color4ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem color5ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem color6ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem color7ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem color8ToolStripMenuItem;
        private System.Windows.Forms.ToolStripMenuItem color9ToolStripMenuItem;
        private System.Windows.Forms.DataGridView dataGridView2;
        private System.Windows.Forms.DataGridViewImageColumn Column2;
        private System.Windows.Forms.DataGridView dataGridView1;
        private System.Windows.Forms.DataGridViewImageColumn Column1;
        private MaterialSkin.Controls.MaterialFlatButton startBtn;
        private System.Windows.Forms.ImageList imageList1;
    }
}

