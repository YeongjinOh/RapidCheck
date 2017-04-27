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
            this.materialTabControl1 = new MaterialSkin.Controls.MaterialTabControl();
            this.tabPage1 = new System.Windows.Forms.TabPage();
            this.pictureBox1 = new System.Windows.Forms.PictureBox();
            this.tabPage2 = new System.Windows.Forms.TabPage();
            this.tabPage3 = new System.Windows.Forms.TabPage();
            this.videoPanel = new System.Windows.Forms.Panel();
            this.materialTabSelector1 = new MaterialSkin.Controls.MaterialTabSelector();
            this.openFileDialog1 = new System.Windows.Forms.OpenFileDialog();
            this.materialLabel1 = new MaterialSkin.Controls.MaterialLabel();
            this.CMDBtn = new MaterialSkin.Controls.MaterialFlatButton();
            this.materialRadioButton1 = new MaterialSkin.Controls.MaterialRadioButton();
            this.materialRadioButton2 = new MaterialSkin.Controls.MaterialRadioButton();
            this.materialCheckBox1 = new MaterialSkin.Controls.MaterialCheckBox();
            this.sqlBtn = new MaterialSkin.Controls.MaterialFlatButton();
            this.VideoBtn = new MaterialSkin.Controls.MaterialFlatButton();
            this.overBtn = new System.Windows.Forms.Button();
            this.SQLAdapter = new MaterialSkin.Controls.MaterialFlatButton();
            this.DirectShowBtn = new MaterialSkin.Controls.MaterialFlatButton();
            this.textBox1 = new System.Windows.Forms.TextBox();
            this.captureBtn = new MaterialSkin.Controls.MaterialFlatButton();
            this.materialTabControl1.SuspendLayout();
            this.tabPage1.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).BeginInit();
            this.tabPage3.SuspendLayout();
            this.SuspendLayout();
            // 
            // materialTabControl1
            // 
            this.materialTabControl1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.materialTabControl1.Controls.Add(this.tabPage1);
            this.materialTabControl1.Controls.Add(this.tabPage2);
            this.materialTabControl1.Controls.Add(this.tabPage3);
            this.materialTabControl1.Depth = 0;
            this.materialTabControl1.Font = new System.Drawing.Font("Microsoft Sans Serif", 8.25F);
            this.materialTabControl1.Location = new System.Drawing.Point(12, 200);
            this.materialTabControl1.MouseState = MaterialSkin.MouseState.HOVER;
            this.materialTabControl1.Name = "materialTabControl1";
            this.materialTabControl1.SelectedIndex = 0;
            this.materialTabControl1.Size = new System.Drawing.Size(908, 323);
            this.materialTabControl1.TabIndex = 0;
            // 
            // tabPage1
            // 
            this.tabPage1.Controls.Add(this.pictureBox1);
            this.tabPage1.Location = new System.Drawing.Point(4, 26);
            this.tabPage1.Name = "tabPage1";
            this.tabPage1.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage1.Size = new System.Drawing.Size(900, 293);
            this.tabPage1.TabIndex = 0;
            this.tabPage1.Text = "tabPage1";
            this.tabPage1.UseVisualStyleBackColor = true;
            // 
            // pictureBox1
            // 
            this.pictureBox1.Dock = System.Windows.Forms.DockStyle.Fill;
            this.pictureBox1.Location = new System.Drawing.Point(3, 3);
            this.pictureBox1.Name = "pictureBox1";
            this.pictureBox1.Size = new System.Drawing.Size(894, 287);
            this.pictureBox1.SizeMode = System.Windows.Forms.PictureBoxSizeMode.AutoSize;
            this.pictureBox1.TabIndex = 0;
            this.pictureBox1.TabStop = false;
            // 
            // tabPage2
            // 
            this.tabPage2.Location = new System.Drawing.Point(4, 26);
            this.tabPage2.Name = "tabPage2";
            this.tabPage2.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage2.Size = new System.Drawing.Size(900, 293);
            this.tabPage2.TabIndex = 1;
            this.tabPage2.Text = "tabPage2";
            this.tabPage2.UseVisualStyleBackColor = true;
            // 
            // tabPage3
            // 
            this.tabPage3.Controls.Add(this.videoPanel);
            this.tabPage3.Location = new System.Drawing.Point(4, 26);
            this.tabPage3.Name = "tabPage3";
            this.tabPage3.Padding = new System.Windows.Forms.Padding(3);
            this.tabPage3.Size = new System.Drawing.Size(900, 293);
            this.tabPage3.TabIndex = 2;
            this.tabPage3.Text = "tabPage3";
            this.tabPage3.UseVisualStyleBackColor = true;
            // 
            // videoPanel
            // 
            this.videoPanel.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom) 
            | System.Windows.Forms.AnchorStyles.Left) 
            | System.Windows.Forms.AnchorStyles.Right)));
            this.videoPanel.Location = new System.Drawing.Point(3, 3);
            this.videoPanel.Name = "videoPanel";
            this.videoPanel.Size = new System.Drawing.Size(894, 287);
            this.videoPanel.TabIndex = 1;
            // 
            // materialTabSelector1
            // 
            this.materialTabSelector1.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Left | System.Windows.Forms.AnchorStyles.Right)));
            this.materialTabSelector1.BaseTabControl = this.materialTabControl1;
            this.materialTabSelector1.Depth = 0;
            this.materialTabSelector1.Location = new System.Drawing.Point(12, 163);
            this.materialTabSelector1.MouseState = MaterialSkin.MouseState.HOVER;
            this.materialTabSelector1.Name = "materialTabSelector1";
            this.materialTabSelector1.Size = new System.Drawing.Size(904, 31);
            this.materialTabSelector1.TabIndex = 1;
            this.materialTabSelector1.Text = "materialTabSelector1";
            // 
            // openFileDialog1
            // 
            this.openFileDialog1.FileName = "openFileDialog1";
            // 
            // materialLabel1
            // 
            this.materialLabel1.AutoSize = true;
            this.materialLabel1.Depth = 0;
            this.materialLabel1.Font = new System.Drawing.Font("Roboto", 11F);
            this.materialLabel1.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(222)))), ((int)(((byte)(0)))), ((int)(((byte)(0)))), ((int)(((byte)(0)))));
            this.materialLabel1.Location = new System.Drawing.Point(12, 79);
            this.materialLabel1.MouseState = MaterialSkin.MouseState.HOVER;
            this.materialLabel1.Name = "materialLabel1";
            this.materialLabel1.Size = new System.Drawing.Size(102, 24);
            this.materialLabel1.TabIndex = 3;
            this.materialLabel1.Text = "Just..Label";
            // 
            // CMDBtn
            // 
            this.CMDBtn.AutoSize = true;
            this.CMDBtn.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
            this.CMDBtn.Depth = 0;
            this.CMDBtn.Location = new System.Drawing.Point(12, 116);
            this.CMDBtn.Margin = new System.Windows.Forms.Padding(4, 6, 4, 6);
            this.CMDBtn.MouseState = MaterialSkin.MouseState.HOVER;
            this.CMDBtn.Name = "CMDBtn";
            this.CMDBtn.Primary = false;
            this.CMDBtn.Size = new System.Drawing.Size(51, 36);
            this.CMDBtn.TabIndex = 4;
            this.CMDBtn.Text = "CMD";
            this.CMDBtn.UseVisualStyleBackColor = true;
            this.CMDBtn.Click += new System.EventHandler(this.CMDBtn_Click);
            // 
            // materialRadioButton1
            // 
            this.materialRadioButton1.AutoSize = true;
            this.materialRadioButton1.Depth = 0;
            this.materialRadioButton1.Font = new System.Drawing.Font("Roboto", 10F);
            this.materialRadioButton1.Location = new System.Drawing.Point(127, 76);
            this.materialRadioButton1.Margin = new System.Windows.Forms.Padding(0);
            this.materialRadioButton1.MouseLocation = new System.Drawing.Point(-1, -1);
            this.materialRadioButton1.MouseState = MaterialSkin.MouseState.HOVER;
            this.materialRadioButton1.Name = "materialRadioButton1";
            this.materialRadioButton1.Ripple = true;
            this.materialRadioButton1.Size = new System.Drawing.Size(68, 30);
            this.materialRadioButton1.TabIndex = 5;
            this.materialRadioButton1.TabStop = true;
            this.materialRadioButton1.Text = "사람";
            this.materialRadioButton1.UseVisualStyleBackColor = true;
            // 
            // materialRadioButton2
            // 
            this.materialRadioButton2.AutoSize = true;
            this.materialRadioButton2.Depth = 0;
            this.materialRadioButton2.Font = new System.Drawing.Font("Roboto", 10F);
            this.materialRadioButton2.Location = new System.Drawing.Point(127, 106);
            this.materialRadioButton2.Margin = new System.Windows.Forms.Padding(0);
            this.materialRadioButton2.MouseLocation = new System.Drawing.Point(-1, -1);
            this.materialRadioButton2.MouseState = MaterialSkin.MouseState.HOVER;
            this.materialRadioButton2.Name = "materialRadioButton2";
            this.materialRadioButton2.Ripple = true;
            this.materialRadioButton2.Size = new System.Drawing.Size(87, 30);
            this.materialRadioButton2.TabIndex = 5;
            this.materialRadioButton2.TabStop = true;
            this.materialRadioButton2.Text = "자동차";
            this.materialRadioButton2.UseVisualStyleBackColor = true;
            // 
            // materialCheckBox1
            // 
            this.materialCheckBox1.AutoSize = true;
            this.materialCheckBox1.Depth = 0;
            this.materialCheckBox1.Font = new System.Drawing.Font("Roboto", 10F);
            this.materialCheckBox1.Location = new System.Drawing.Point(224, 77);
            this.materialCheckBox1.Margin = new System.Windows.Forms.Padding(0);
            this.materialCheckBox1.MouseLocation = new System.Drawing.Point(-1, -1);
            this.materialCheckBox1.MouseState = MaterialSkin.MouseState.HOVER;
            this.materialCheckBox1.Name = "materialCheckBox1";
            this.materialCheckBox1.Ripple = true;
            this.materialCheckBox1.Size = new System.Drawing.Size(108, 30);
            this.materialCheckBox1.TabIndex = 6;
            this.materialCheckBox1.Text = "CheckBox";
            this.materialCheckBox1.UseVisualStyleBackColor = true;
            // 
            // sqlBtn
            // 
            this.sqlBtn.AutoSize = true;
            this.sqlBtn.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
            this.sqlBtn.Depth = 0;
            this.sqlBtn.Location = new System.Drawing.Point(362, 113);
            this.sqlBtn.Margin = new System.Windows.Forms.Padding(4, 6, 4, 6);
            this.sqlBtn.MouseState = MaterialSkin.MouseState.HOVER;
            this.sqlBtn.Name = "sqlBtn";
            this.sqlBtn.Primary = false;
            this.sqlBtn.Size = new System.Drawing.Size(45, 36);
            this.sqlBtn.TabIndex = 7;
            this.sqlBtn.Text = "SQL";
            this.sqlBtn.UseVisualStyleBackColor = true;
            this.sqlBtn.Click += new System.EventHandler(this.sqlBtn_Click);
            // 
            // VideoBtn
            // 
            this.VideoBtn.AutoSize = true;
            this.VideoBtn.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
            this.VideoBtn.Depth = 0;
            this.VideoBtn.Location = new System.Drawing.Point(508, 76);
            this.VideoBtn.Margin = new System.Windows.Forms.Padding(4, 6, 4, 6);
            this.VideoBtn.MouseState = MaterialSkin.MouseState.HOVER;
            this.VideoBtn.Name = "VideoBtn";
            this.VideoBtn.Primary = false;
            this.VideoBtn.Size = new System.Drawing.Size(62, 36);
            this.VideoBtn.TabIndex = 7;
            this.VideoBtn.Text = "Video";
            this.VideoBtn.UseVisualStyleBackColor = true;
            this.VideoBtn.Click += new System.EventHandler(this.VideoBtn_Click);
            // 
            // overBtn
            // 
            this.overBtn.Location = new System.Drawing.Point(508, 123);
            this.overBtn.Name = "overBtn";
            this.overBtn.Size = new System.Drawing.Size(136, 34);
            this.overBtn.TabIndex = 8;
            this.overBtn.Text = "overlay test";
            this.overBtn.UseVisualStyleBackColor = true;
            this.overBtn.Click += new System.EventHandler(this.overBtn_Click);
            // 
            // SQLAdapter
            // 
            this.SQLAdapter.AutoSize = true;
            this.SQLAdapter.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
            this.SQLAdapter.Depth = 0;
            this.SQLAdapter.Location = new System.Drawing.Point(362, 73);
            this.SQLAdapter.Margin = new System.Windows.Forms.Padding(4, 6, 4, 6);
            this.SQLAdapter.MouseState = MaterialSkin.MouseState.HOVER;
            this.SQLAdapter.Name = "SQLAdapter";
            this.SQLAdapter.Primary = false;
            this.SQLAdapter.Size = new System.Drawing.Size(120, 36);
            this.SQLAdapter.TabIndex = 7;
            this.SQLAdapter.Text = "SQLAdapter";
            this.SQLAdapter.UseVisualStyleBackColor = true;
            this.SQLAdapter.Click += new System.EventHandler(this.sqlAdapterBtn_Click);
            // 
            // DirectShowBtn
            // 
            this.DirectShowBtn.AutoSize = true;
            this.DirectShowBtn.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
            this.DirectShowBtn.Depth = 0;
            this.DirectShowBtn.Location = new System.Drawing.Point(707, 79);
            this.DirectShowBtn.Margin = new System.Windows.Forms.Padding(4, 6, 4, 6);
            this.DirectShowBtn.MouseState = MaterialSkin.MouseState.HOVER;
            this.DirectShowBtn.Name = "DirectShowBtn";
            this.DirectShowBtn.Primary = false;
            this.DirectShowBtn.Size = new System.Drawing.Size(79, 36);
            this.DirectShowBtn.TabIndex = 7;
            this.DirectShowBtn.Text = "DiShow";
            this.DirectShowBtn.UseVisualStyleBackColor = true;
            this.DirectShowBtn.Click += new System.EventHandler(this.Video2Btn_Click);
            // 
            // textBox1
            // 
            this.textBox1.Location = new System.Drawing.Point(707, 121);
            this.textBox1.Name = "textBox1";
            this.textBox1.Size = new System.Drawing.Size(106, 27);
            this.textBox1.TabIndex = 9;
            // 
            // captureBtn
            // 
            this.captureBtn.AutoSize = true;
            this.captureBtn.AutoSizeMode = System.Windows.Forms.AutoSizeMode.GrowAndShrink;
            this.captureBtn.Depth = 0;
            this.captureBtn.Location = new System.Drawing.Point(794, 79);
            this.captureBtn.Margin = new System.Windows.Forms.Padding(4, 6, 4, 6);
            this.captureBtn.MouseState = MaterialSkin.MouseState.HOVER;
            this.captureBtn.Name = "captureBtn";
            this.captureBtn.Primary = false;
            this.captureBtn.Size = new System.Drawing.Size(89, 36);
            this.captureBtn.TabIndex = 10;
            this.captureBtn.Text = "capture";
            this.captureBtn.UseVisualStyleBackColor = true;
            this.captureBtn.Click += new System.EventHandler(this.captureBtn_Click);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 20F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(932, 523);
            this.Controls.Add(this.captureBtn);
            this.Controls.Add(this.textBox1);
            this.Controls.Add(this.overBtn);
            this.Controls.Add(this.DirectShowBtn);
            this.Controls.Add(this.VideoBtn);
            this.Controls.Add(this.SQLAdapter);
            this.Controls.Add(this.sqlBtn);
            this.Controls.Add(this.materialCheckBox1);
            this.Controls.Add(this.materialRadioButton2);
            this.Controls.Add(this.materialRadioButton1);
            this.Controls.Add(this.CMDBtn);
            this.Controls.Add(this.materialLabel1);
            this.Controls.Add(this.materialTabSelector1);
            this.Controls.Add(this.materialTabControl1);
            this.Font = new System.Drawing.Font("Segoe UI Symbol", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.Margin = new System.Windows.Forms.Padding(3, 4, 3, 4);
            this.Name = "Form1";
            this.Text = "RapidCheck";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.materialTabControl1.ResumeLayout(false);
            this.tabPage1.ResumeLayout(false);
            this.tabPage1.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.pictureBox1)).EndInit();
            this.tabPage3.ResumeLayout(false);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private MaterialSkin.Controls.MaterialTabControl materialTabControl1;
        private System.Windows.Forms.TabPage tabPage1;
        private System.Windows.Forms.TabPage tabPage2;
        private MaterialSkin.Controls.MaterialTabSelector materialTabSelector1;
        private System.Windows.Forms.TabPage tabPage3;
        private System.Windows.Forms.Panel videoPanel;
        private System.Windows.Forms.OpenFileDialog openFileDialog1;
        private MaterialSkin.Controls.MaterialLabel materialLabel1;
        private MaterialSkin.Controls.MaterialFlatButton CMDBtn;
        private MaterialSkin.Controls.MaterialRadioButton materialRadioButton1;
        private MaterialSkin.Controls.MaterialRadioButton materialRadioButton2;
        private MaterialSkin.Controls.MaterialCheckBox materialCheckBox1;
        private MaterialSkin.Controls.MaterialFlatButton sqlBtn;
        private MaterialSkin.Controls.MaterialFlatButton VideoBtn;
        private System.Windows.Forms.Button overBtn;
        private System.Windows.Forms.PictureBox pictureBox1;
        private MaterialSkin.Controls.MaterialFlatButton SQLAdapter;
        private MaterialSkin.Controls.MaterialFlatButton DirectShowBtn;
        private System.Windows.Forms.TextBox textBox1;
        private MaterialSkin.Controls.MaterialFlatButton captureBtn;
    }
}

