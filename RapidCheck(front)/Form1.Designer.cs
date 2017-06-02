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
            this.timeLabel = new System.Windows.Forms.Label();
            this.progressBar1 = new System.Windows.Forms.ProgressBar();
            this.stateLabel = new System.Windows.Forms.Label();
            this.panelProgress = new System.Windows.Forms.Panel();
            this.VideoBtn = new System.Windows.Forms.Button();
            this.panelVideo = new System.Windows.Forms.Panel();
            this.panelVideoPart = new System.Windows.Forms.Panel();
            this.pictureBoxVideo = new System.Windows.Forms.PictureBox();
            this.startBtn = new System.Windows.Forms.Button();
            this.trackBar1 = new System.Windows.Forms.TrackBar();
            this.panelSearch = new System.Windows.Forms.Panel();
            this.tabPage3 = new System.Windows.Forms.TabPage();
            this.videoPanel = new System.Windows.Forms.Panel();
            this.axWindowsMediaPlayer1 = new AxWMPLib.AxWindowsMediaPlayer();
            this.tabPage1 = new System.Windows.Forms.TabPage();
            this.direction5 = new System.Windows.Forms.Button();
            this.direction6 = new System.Windows.Forms.Button();
            this.direction4 = new System.Windows.Forms.Button();
            this.direction = new System.Windows.Forms.Button();
            this.direction7 = new System.Windows.Forms.Button();
            this.direction3 = new System.Windows.Forms.Button();
            this.direction8 = new System.Windows.Forms.Button();
            this.direction2 = new System.Windows.Forms.Button();
            this.direction1 = new System.Windows.Forms.Button();
            this.radioButton9 = new System.Windows.Forms.RadioButton();
            this.radioButton8 = new System.Windows.Forms.RadioButton();
            this.radioButton7 = new System.Windows.Forms.RadioButton();
            this.radioButton6 = new System.Windows.Forms.RadioButton();
            this.radioButton5 = new System.Windows.Forms.RadioButton();
            this.radioButton4 = new System.Windows.Forms.RadioButton();
            this.radioButton3 = new System.Windows.Forms.RadioButton();
            this.radioButton2 = new System.Windows.Forms.RadioButton();
            this.radioButton10 = new System.Windows.Forms.RadioButton();
            this.radioButton1 = new System.Windows.Forms.RadioButton();
            this.pictureBox1 = new System.Windows.Forms.PictureBox();
            this.materialTabSelector1 = new MaterialSkin.Controls.MaterialTabSelector();
            this.openFileDialog1 = new System.Windows.Forms.OpenFileDialog();
            this.colorDialog1 = new System.Windows.Forms.ColorDialog();
            this.timer1 = new System.Windows.Forms.Timer(this.components);
            this.comboBox1 = new System.Windows.Forms.ComboBox();
            this.materialTabControl1.SuspendLayout();
            this.tabPage2.SuspendLayout();
            this.panelObject.SuspendLayout();
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
            this.SuspendLayout();
            // 
            // materialTabControl1
            // 
            this.materialTabControl1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.materialTabControl1.Controls.Add(this.tabPage2);
            this.materialTabControl1.Controls.Add(this.tabPage3);
            this.materialTabControl1.Controls.Add(this.tabPage1);
            this.materialTabControl1.Depth = 0;
            this.materialTabControl1.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F);
            this.materialTabControl1.Location = new System.Drawing.Point(12, 103);
            this.materialTabControl1.MouseState = MaterialSkin.MouseState.HOVER;
            this.materialTabControl1.Name = "materialTabControl1";
            this.materialTabControl1.SelectedIndex = 0;
            this.materialTabControl1.Size = new System.Drawing.Size(1524, 773);
            this.materialTabControl1.TabIndex = 0;
            // 
            // tabPage2
            // 
            this.tabPage2.Controls.Add(this.panelObject);
            this.tabPage2.Controls.Add(this.panelProgress);
            this.tabPage2.Controls.Add(this.panelVideo);
            this.tabPage2.Location = new System.Drawing.Point(4, 26);
            this.tabPage2.Name = "tabPage2";
            this.tabPage2.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage2.Size = new System.Drawing.Size(1516, 743);
            this.tabPage2.TabIndex = 1;
            this.tabPage2.Text = "tabPage2";
            this.tabPage2.UseVisualStyleBackColor = true;
            // 
            // panelObject
            // 
            this.panelObject.BackColor = System.Drawing.Color.Transparent;
            this.panelObject.Controls.Add(this.timeLabel);
            this.panelObject.Controls.Add(this.stateLabel);
            this.panelObject.Dock = System.Windows.Forms.DockStyle.Fill;
            this.panelObject.Location = new System.Drawing.Point(1075, 70);
            this.panelObject.Name = "panelObject";
            this.panelObject.Size = new System.Drawing.Size(438, 670);
            this.panelObject.TabIndex = 2;
            // 
            // timeLabel
            // 
            this.timeLabel.AutoSize = true;
            this.timeLabel.Location = new System.Drawing.Point(6, 83);
            this.timeLabel.Name = "timeLabel";
            this.timeLabel.Size = new System.Drawing.Size(39, 17);
            this.timeLabel.TabIndex = 15;
            this.timeLabel.Text = "Time";
            // 
            // progressBar1
            // 
            this.progressBar1.BackColor = System.Drawing.SystemColors.ActiveCaption;
            this.progressBar1.Location = new System.Drawing.Point(78, 15);
            this.progressBar1.Name = "progressBar1";
            this.progressBar1.Size = new System.Drawing.Size(357, 34);
            this.progressBar1.TabIndex = 12;
            // 
            // stateLabel
            // 
            this.stateLabel.AutoSize = true;
            this.stateLabel.Font = new System.Drawing.Font("Microsoft Sans Serif", 19.8F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.stateLabel.Location = new System.Drawing.Point(2, 31);
            this.stateLabel.Name = "stateLabel";
            this.stateLabel.Size = new System.Drawing.Size(88, 38);
            this.stateLabel.TabIndex = 14;
            this.stateLabel.Text = "state";
            // 
            // panelProgress
            // 
            this.panelProgress.BackColor = System.Drawing.Color.Transparent;
            this.panelProgress.Controls.Add(this.VideoBtn);
            this.panelProgress.Controls.Add(this.progressBar1);
            this.panelProgress.Dock = System.Windows.Forms.DockStyle.Top;
            this.panelProgress.Location = new System.Drawing.Point(1075, 3);
            this.panelProgress.Name = "panelProgress";
            this.panelProgress.Size = new System.Drawing.Size(438, 67);
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
            // panelVideo
            // 
            this.panelVideo.BackColor = System.Drawing.Color.White;
            this.panelVideo.Controls.Add(this.comboBox1);
            this.panelVideo.Controls.Add(this.panelVideoPart);
            this.panelVideo.Controls.Add(this.startBtn);
            this.panelVideo.Controls.Add(this.trackBar1);
            this.panelVideo.Controls.Add(this.panelSearch);
            this.panelVideo.Dock = System.Windows.Forms.DockStyle.Left;
            this.panelVideo.Location = new System.Drawing.Point(3, 3);
            this.panelVideo.Name = "panelVideo";
            this.panelVideo.Size = new System.Drawing.Size(1072, 737);
            this.panelVideo.TabIndex = 0;
            // 
            // panelVideoPart
            // 
            this.panelVideoPart.Controls.Add(this.pictureBoxVideo);
            this.panelVideoPart.Dock = System.Windows.Forms.DockStyle.Top;
            this.panelVideoPart.Location = new System.Drawing.Point(0, 67);
            this.panelVideoPart.Name = "panelVideoPart";
            this.panelVideoPart.Size = new System.Drawing.Size(1072, 621);
            this.panelVideoPart.TabIndex = 3;
            // 
            // pictureBoxVideo
            // 
            this.pictureBoxVideo.BackColor = System.Drawing.Color.Transparent;
            this.pictureBoxVideo.Location = new System.Drawing.Point(0, 0);
            this.pictureBoxVideo.Name = "pictureBoxVideo";
            this.pictureBoxVideo.Size = new System.Drawing.Size(1072, 621);
            this.pictureBoxVideo.TabIndex = 1;
            this.pictureBoxVideo.TabStop = false;
            this.pictureBoxVideo.MouseDown += new System.Windows.Forms.MouseEventHandler(this.pictureBoxVideo_MouseDown);
            // 
            // startBtn
            // 
            this.startBtn.Enabled = false;
            this.startBtn.Location = new System.Drawing.Point(172, 709);
            this.startBtn.Name = "startBtn";
            this.startBtn.Size = new System.Drawing.Size(89, 26);
            this.startBtn.TabIndex = 2;
            this.startBtn.Text = "Start";
            this.startBtn.UseVisualStyleBackColor = true;
            this.startBtn.Click += new System.EventHandler(this.VideoStartBtn_Click);
            // 
            // trackBar1
            // 
            this.trackBar1.BackColor = System.Drawing.Color.White;
            this.trackBar1.Enabled = false;
            this.trackBar1.Location = new System.Drawing.Point(2, 689);
            this.trackBar1.Name = "trackBar1";
            this.trackBar1.Size = new System.Drawing.Size(1069, 56);
            this.trackBar1.TabIndex = 2;
            this.trackBar1.TickStyle = System.Windows.Forms.TickStyle.None;
            this.trackBar1.ValueChanged += new System.EventHandler(this.trackBar1_ValueChanged);
            this.trackBar1.MouseDown += new System.Windows.Forms.MouseEventHandler(this.trackBar1_MouseDown);
            // 
            // panelSearch
            // 
            this.panelSearch.BackColor = System.Drawing.Color.Transparent;
            this.panelSearch.Dock = System.Windows.Forms.DockStyle.Top;
            this.panelSearch.Location = new System.Drawing.Point(0, 0);
            this.panelSearch.Name = "panelSearch";
            this.panelSearch.Size = new System.Drawing.Size(1072, 67);
            this.panelSearch.TabIndex = 0;
            // 
            // tabPage3
            // 
            this.tabPage3.Controls.Add(this.videoPanel);
            this.tabPage3.Location = new System.Drawing.Point(4, 26);
            this.tabPage3.Name = "tabPage3";
            this.tabPage3.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage3.Size = new System.Drawing.Size(1516, 743);
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
            this.videoPanel.Size = new System.Drawing.Size(1510, 737);
            this.videoPanel.TabIndex = 1;
            // 
            // axWindowsMediaPlayer1
            // 
            this.axWindowsMediaPlayer1.Enabled = true;
            this.axWindowsMediaPlayer1.Location = new System.Drawing.Point(3, 0);
            this.axWindowsMediaPlayer1.Name = "axWindowsMediaPlayer1";
            this.axWindowsMediaPlayer1.OcxState = ((System.Windows.Forms.AxHost.State)(resources.GetObject("axWindowsMediaPlayer1.OcxState")));
            this.axWindowsMediaPlayer1.Size = new System.Drawing.Size(1507, 737);
            this.axWindowsMediaPlayer1.TabIndex = 0;
            // 
            // tabPage1
            // 
            this.tabPage1.Controls.Add(this.direction5);
            this.tabPage1.Controls.Add(this.direction6);
            this.tabPage1.Controls.Add(this.direction4);
            this.tabPage1.Controls.Add(this.direction);
            this.tabPage1.Controls.Add(this.direction7);
            this.tabPage1.Controls.Add(this.direction3);
            this.tabPage1.Controls.Add(this.direction8);
            this.tabPage1.Controls.Add(this.direction2);
            this.tabPage1.Controls.Add(this.direction1);
            this.tabPage1.Controls.Add(this.radioButton9);
            this.tabPage1.Controls.Add(this.radioButton8);
            this.tabPage1.Controls.Add(this.radioButton7);
            this.tabPage1.Controls.Add(this.radioButton6);
            this.tabPage1.Controls.Add(this.radioButton5);
            this.tabPage1.Controls.Add(this.radioButton4);
            this.tabPage1.Controls.Add(this.radioButton3);
            this.tabPage1.Controls.Add(this.radioButton2);
            this.tabPage1.Controls.Add(this.radioButton10);
            this.tabPage1.Controls.Add(this.radioButton1);
            this.tabPage1.Controls.Add(this.pictureBox1);
            this.tabPage1.Location = new System.Drawing.Point(4, 26);
            this.tabPage1.Name = "tabPage1";
            this.tabPage1.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage1.Size = new System.Drawing.Size(1516, 743);
            this.tabPage1.TabIndex = 0;
            this.tabPage1.Text = "tabPage1";
            this.tabPage1.UseVisualStyleBackColor = true;
            // 
            // direction5
            // 
            this.direction5.Location = new System.Drawing.Point(831, 138);
            this.direction5.Name = "direction5";
            this.direction5.Size = new System.Drawing.Size(46, 46);
            this.direction5.TabIndex = 2;
            this.direction5.Text = "↘";
            this.direction5.UseVisualStyleBackColor = true;
            this.direction5.Click += new System.EventHandler(this.direction5_Click);
            // 
            // direction6
            // 
            this.direction6.Location = new System.Drawing.Point(779, 139);
            this.direction6.Name = "direction6";
            this.direction6.Size = new System.Drawing.Size(46, 46);
            this.direction6.TabIndex = 2;
            this.direction6.Text = "↓";
            this.direction6.UseVisualStyleBackColor = true;
            this.direction6.Click += new System.EventHandler(this.direction6_Click);
            // 
            // direction4
            // 
            this.direction4.Location = new System.Drawing.Point(831, 86);
            this.direction4.Name = "direction4";
            this.direction4.Size = new System.Drawing.Size(46, 46);
            this.direction4.TabIndex = 2;
            this.direction4.Text = "→";
            this.direction4.UseVisualStyleBackColor = true;
            this.direction4.Click += new System.EventHandler(this.direction4_Click);
            // 
            // direction
            // 
            this.direction.Location = new System.Drawing.Point(779, 87);
            this.direction.Name = "direction";
            this.direction.Size = new System.Drawing.Size(46, 46);
            this.direction.TabIndex = 2;
            this.direction.Text = "direction";
            this.direction.UseVisualStyleBackColor = true;
            // 
            // direction7
            // 
            this.direction7.Location = new System.Drawing.Point(727, 139);
            this.direction7.Name = "direction7";
            this.direction7.Size = new System.Drawing.Size(46, 46);
            this.direction7.TabIndex = 2;
            this.direction7.Text = "↙";
            this.direction7.UseVisualStyleBackColor = true;
            this.direction7.Click += new System.EventHandler(this.direction7_Click);
            // 
            // direction3
            // 
            this.direction3.Location = new System.Drawing.Point(831, 34);
            this.direction3.Name = "direction3";
            this.direction3.Size = new System.Drawing.Size(46, 46);
            this.direction3.TabIndex = 2;
            this.direction3.Text = "↗";
            this.direction3.UseVisualStyleBackColor = true;
            this.direction3.Click += new System.EventHandler(this.direction3_Click);
            // 
            // direction8
            // 
            this.direction8.Location = new System.Drawing.Point(727, 87);
            this.direction8.Name = "direction8";
            this.direction8.Size = new System.Drawing.Size(46, 46);
            this.direction8.TabIndex = 2;
            this.direction8.Text = "←";
            this.direction8.UseVisualStyleBackColor = true;
            this.direction8.Click += new System.EventHandler(this.direction8_Click);
            // 
            // direction2
            // 
            this.direction2.Location = new System.Drawing.Point(779, 35);
            this.direction2.Name = "direction2";
            this.direction2.Size = new System.Drawing.Size(46, 46);
            this.direction2.TabIndex = 2;
            this.direction2.Text = "↑";
            this.direction2.UseVisualStyleBackColor = true;
            this.direction2.Click += new System.EventHandler(this.direction2_Click);
            // 
            // direction1
            // 
            this.direction1.Location = new System.Drawing.Point(727, 35);
            this.direction1.Name = "direction1";
            this.direction1.Size = new System.Drawing.Size(46, 46);
            this.direction1.TabIndex = 2;
            this.direction1.Text = "↖";
            this.direction1.UseVisualStyleBackColor = true;
            this.direction1.Click += new System.EventHandler(this.direction1_Click);
            // 
            // radioButton9
            // 
            this.radioButton9.AutoSize = true;
            this.radioButton9.Checked = true;
            this.radioButton9.Location = new System.Drawing.Point(571, 125);
            this.radioButton9.Name = "radioButton9";
            this.radioButton9.Size = new System.Drawing.Size(110, 21);
            this.radioButton9.TabIndex = 1;
            this.radioButton9.TabStop = true;
            this.radioButton9.Text = "radioButton1";
            this.radioButton9.UseVisualStyleBackColor = true;
            this.radioButton9.CheckedChanged += new System.EventHandler(this.radioButton9_CheckedChanged);
            // 
            // radioButton8
            // 
            this.radioButton8.AutoSize = true;
            this.radioButton8.Location = new System.Drawing.Point(571, 98);
            this.radioButton8.Name = "radioButton8";
            this.radioButton8.Size = new System.Drawing.Size(110, 21);
            this.radioButton8.TabIndex = 1;
            this.radioButton8.Text = "radioButton1";
            this.radioButton8.UseVisualStyleBackColor = true;
            this.radioButton8.CheckedChanged += new System.EventHandler(this.radioButton8_CheckedChanged);
            // 
            // radioButton7
            // 
            this.radioButton7.AutoSize = true;
            this.radioButton7.Location = new System.Drawing.Point(571, 71);
            this.radioButton7.Name = "radioButton7";
            this.radioButton7.Size = new System.Drawing.Size(110, 21);
            this.radioButton7.TabIndex = 1;
            this.radioButton7.Text = "radioButton1";
            this.radioButton7.UseVisualStyleBackColor = true;
            this.radioButton7.CheckedChanged += new System.EventHandler(this.radioButton7_CheckedChanged);
            // 
            // radioButton6
            // 
            this.radioButton6.AutoSize = true;
            this.radioButton6.Location = new System.Drawing.Point(571, 44);
            this.radioButton6.Name = "radioButton6";
            this.radioButton6.Size = new System.Drawing.Size(110, 21);
            this.radioButton6.TabIndex = 1;
            this.radioButton6.Text = "radioButton1";
            this.radioButton6.UseVisualStyleBackColor = true;
            this.radioButton6.CheckedChanged += new System.EventHandler(this.radioButton6_CheckedChanged);
            // 
            // radioButton5
            // 
            this.radioButton5.AutoSize = true;
            this.radioButton5.Location = new System.Drawing.Point(458, 152);
            this.radioButton5.Name = "radioButton5";
            this.radioButton5.Size = new System.Drawing.Size(110, 21);
            this.radioButton5.TabIndex = 1;
            this.radioButton5.Text = "radioButton1";
            this.radioButton5.UseVisualStyleBackColor = true;
            this.radioButton5.CheckedChanged += new System.EventHandler(this.radioButton5_CheckedChanged);
            // 
            // radioButton4
            // 
            this.radioButton4.AutoSize = true;
            this.radioButton4.Location = new System.Drawing.Point(458, 125);
            this.radioButton4.Name = "radioButton4";
            this.radioButton4.Size = new System.Drawing.Size(110, 21);
            this.radioButton4.TabIndex = 1;
            this.radioButton4.Text = "radioButton1";
            this.radioButton4.UseVisualStyleBackColor = true;
            this.radioButton4.CheckedChanged += new System.EventHandler(this.radioButton4_CheckedChanged);
            // 
            // radioButton3
            // 
            this.radioButton3.AutoSize = true;
            this.radioButton3.Location = new System.Drawing.Point(458, 98);
            this.radioButton3.Name = "radioButton3";
            this.radioButton3.Size = new System.Drawing.Size(110, 21);
            this.radioButton3.TabIndex = 1;
            this.radioButton3.Text = "radioButton1";
            this.radioButton3.UseVisualStyleBackColor = true;
            this.radioButton3.CheckedChanged += new System.EventHandler(this.radioButton3_CheckedChanged);
            // 
            // radioButton2
            // 
            this.radioButton2.AutoSize = true;
            this.radioButton2.Location = new System.Drawing.Point(458, 71);
            this.radioButton2.Name = "radioButton2";
            this.radioButton2.Size = new System.Drawing.Size(110, 21);
            this.radioButton2.TabIndex = 1;
            this.radioButton2.Text = "radioButton1";
            this.radioButton2.UseVisualStyleBackColor = true;
            this.radioButton2.CheckedChanged += new System.EventHandler(this.radioButton2_CheckedChanged);
            // 
            // radioButton10
            // 
            this.radioButton10.AutoSize = true;
            this.radioButton10.Location = new System.Drawing.Point(571, 152);
            this.radioButton10.Name = "radioButton10";
            this.radioButton10.Size = new System.Drawing.Size(110, 21);
            this.radioButton10.TabIndex = 1;
            this.radioButton10.Text = "radioButton1";
            this.radioButton10.UseVisualStyleBackColor = true;
            this.radioButton10.CheckedChanged += new System.EventHandler(this.radioButton10_CheckedChanged);
            // 
            // radioButton1
            // 
            this.radioButton1.AutoSize = true;
            this.radioButton1.Location = new System.Drawing.Point(458, 44);
            this.radioButton1.Name = "radioButton1";
            this.radioButton1.Size = new System.Drawing.Size(110, 21);
            this.radioButton1.TabIndex = 1;
            this.radioButton1.Text = "radioButton1";
            this.radioButton1.UseVisualStyleBackColor = true;
            this.radioButton1.CheckedChanged += new System.EventHandler(this.radioButton1_CheckedChanged);
            // 
            // pictureBox1
            // 
            this.pictureBox1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.pictureBox1.Location = new System.Drawing.Point(3, 3);
            this.pictureBox1.Name = "pictureBox1";
            this.pictureBox1.Size = new System.Drawing.Size(1510, 737);
            this.pictureBox1.SizeMode = System.Windows.Forms.PictureBoxSizeMode.AutoSize;
            this.pictureBox1.TabIndex = 0;
            this.pictureBox1.TabStop = false;
            // 
            // materialTabSelector1
            // 
            this.materialTabSelector1.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.materialTabSelector1.BaseTabControl = this.materialTabControl1;
            this.materialTabSelector1.Depth = 0;
            this.materialTabSelector1.Location = new System.Drawing.Point(0, 65);
            this.materialTabSelector1.MouseState = MaterialSkin.MouseState.HOVER;
            this.materialTabSelector1.Name = "materialTabSelector1";
            this.materialTabSelector1.Size = new System.Drawing.Size(1548, 31);
            this.materialTabSelector1.TabIndex = 1;
            this.materialTabSelector1.Text = "materialTabSelector1";
            // 
            // openFileDialog1
            // 
            this.openFileDialog1.FileName = "openFileDialog1";
            // 
            // comboBox1
            // 
            this.comboBox1.FormattingEnabled = true;
            this.comboBox1.Location = new System.Drawing.Point(267, 710);
            this.comboBox1.Name = "comboBox1";
            this.comboBox1.Size = new System.Drawing.Size(134, 25);
            this.comboBox1.TabIndex = 4;
            this.comboBox1.SelectedIndexChanged += new System.EventHandler(this.comboBox1_SelectedIndexChanged);
            // 
            // Form1
            // 
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.None;
            this.ClientSize = new System.Drawing.Size(1548, 876);
            this.Controls.Add(this.materialTabSelector1);
            this.Controls.Add(this.materialTabControl1);
            this.Font = new System.Drawing.Font("Segoe UI Symbol", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.Name = "Form1";
            this.Text = "RapidCheck";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.Resize += new System.EventHandler(this.Form1_Resize);
            this.materialTabControl1.ResumeLayout(false);
            this.tabPage2.ResumeLayout(false);
            this.panelObject.ResumeLayout(false);
            this.panelObject.PerformLayout();
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
        private System.Windows.Forms.ColorDialog colorDialog1;
        private System.Windows.Forms.RadioButton radioButton7;
        private System.Windows.Forms.RadioButton radioButton6;
        private System.Windows.Forms.RadioButton radioButton5;
        private System.Windows.Forms.RadioButton radioButton4;
        private System.Windows.Forms.RadioButton radioButton3;
        private System.Windows.Forms.RadioButton radioButton2;
        private System.Windows.Forms.RadioButton radioButton1;
        private System.Windows.Forms.RadioButton radioButton9;
        private System.Windows.Forms.RadioButton radioButton8;
        private System.Windows.Forms.RadioButton radioButton10;
        private System.Windows.Forms.Button direction5;
        private System.Windows.Forms.Button direction6;
        private System.Windows.Forms.Button direction4;
        private System.Windows.Forms.Button direction;
        private System.Windows.Forms.Button direction7;
        private System.Windows.Forms.Button direction3;
        private System.Windows.Forms.Button direction8;
        private System.Windows.Forms.Button direction2;
        private System.Windows.Forms.Button direction1;
        private System.Windows.Forms.Timer timer1;
        private System.Windows.Forms.Panel panelObject;
        private System.Windows.Forms.Panel panelProgress;
        private System.Windows.Forms.Panel panelVideo;
        private System.Windows.Forms.Panel panelSearch;
        private System.Windows.Forms.PictureBox pictureBoxVideo;
        private System.Windows.Forms.TrackBar trackBar1;
        private System.Windows.Forms.Button startBtn;
        private System.Windows.Forms.Panel panelVideoPart;
        private System.Windows.Forms.Button VideoBtn;
        private System.Windows.Forms.Label stateLabel;
        private System.Windows.Forms.Label timeLabel;
        private System.Windows.Forms.ProgressBar progressBar1;
        private AxWMPLib.AxWindowsMediaPlayer axWindowsMediaPlayer1;
        private System.Windows.Forms.ComboBox comboBox1;
    }
}

