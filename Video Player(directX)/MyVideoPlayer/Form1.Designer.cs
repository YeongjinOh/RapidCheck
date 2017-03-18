﻿namespace MyVideoPlayer
{
    partial class Form1
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.btnVolume = new System.Windows.Forms.Button();
            this.trackVolume = new System.Windows.Forms.TrackBar();
            this.btnNext = new System.Windows.Forms.Button();
            this.btnPrevious = new System.Windows.Forms.Button();
            this.lblVideoPosition = new System.Windows.Forms.Label();
            this.btnPlayPause = new System.Windows.Forms.Button();
            this.btnFullscreen = new System.Windows.Forms.Button();
            this.lblVideo = new System.Windows.Forms.Label();
            this.lstVideos = new System.Windows.Forms.ListBox();
            this.pnlVideo = new System.Windows.Forms.Panel();
            this.tmrVideo = new System.Windows.Forms.Timer(this.components);
            ((System.ComponentModel.ISupportInitialize)(this.trackVolume)).BeginInit();
            this.SuspendLayout();
            // 
            // btnVolume
            // 
            this.btnVolume.Location = new System.Drawing.Point(560, 528);
            this.btnVolume.Margin = new System.Windows.Forms.Padding(5, 4, 5, 4);
            this.btnVolume.Name = "btnVolume";
            this.btnVolume.Size = new System.Drawing.Size(125, 32);
            this.btnVolume.TabIndex = 20;
            this.btnVolume.Text = "Volume";
            this.btnVolume.UseVisualStyleBackColor = true;
            this.btnVolume.Click += new System.EventHandler(this.btnVolume_Click);
            // 
            // trackVolume
            // 
            this.trackVolume.Location = new System.Drawing.Point(695, 528);
            this.trackVolume.Margin = new System.Windows.Forms.Padding(5, 4, 5, 4);
            this.trackVolume.Maximum = 0;
            this.trackVolume.Minimum = -10000;
            this.trackVolume.Name = "trackVolume";
            this.trackVolume.Size = new System.Drawing.Size(255, 69);
            this.trackVolume.TabIndex = 19;
            this.trackVolume.TickStyle = System.Windows.Forms.TickStyle.None;
            this.trackVolume.Visible = false;
            this.trackVolume.Scroll += new System.EventHandler(this.trackVolume_Scroll);
            // 
            // btnNext
            // 
            this.btnNext.Location = new System.Drawing.Point(290, 528);
            this.btnNext.Margin = new System.Windows.Forms.Padding(5, 4, 5, 4);
            this.btnNext.Name = "btnNext";
            this.btnNext.Size = new System.Drawing.Size(125, 32);
            this.btnNext.TabIndex = 18;
            this.btnNext.Text = "Next";
            this.btnNext.UseVisualStyleBackColor = true;
            this.btnNext.Click += new System.EventHandler(this.btnNext_Click);
            // 
            // btnPrevious
            // 
            this.btnPrevious.Location = new System.Drawing.Point(20, 528);
            this.btnPrevious.Margin = new System.Windows.Forms.Padding(5, 4, 5, 4);
            this.btnPrevious.Name = "btnPrevious";
            this.btnPrevious.Size = new System.Drawing.Size(125, 32);
            this.btnPrevious.TabIndex = 17;
            this.btnPrevious.Text = "Previous";
            this.btnPrevious.UseVisualStyleBackColor = true;
            this.btnPrevious.Click += new System.EventHandler(this.btnPrevious_Click);
            // 
            // lblVideoPosition
            // 
            this.lblVideoPosition.AutoSize = true;
            this.lblVideoPosition.Font = new System.Drawing.Font("Microsoft Sans Serif", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblVideoPosition.Location = new System.Drawing.Point(960, 530);
            this.lblVideoPosition.Margin = new System.Windows.Forms.Padding(5, 0, 5, 0);
            this.lblVideoPosition.Name = "lblVideoPosition";
            this.lblVideoPosition.Size = new System.Drawing.Size(184, 25);
            this.lblVideoPosition.TabIndex = 16;
            this.lblVideoPosition.Text = "00:00:00 / 00:00:00";
            // 
            // btnPlayPause
            // 
            this.btnPlayPause.Location = new System.Drawing.Point(155, 528);
            this.btnPlayPause.Margin = new System.Windows.Forms.Padding(5, 4, 5, 4);
            this.btnPlayPause.Name = "btnPlayPause";
            this.btnPlayPause.Size = new System.Drawing.Size(125, 32);
            this.btnPlayPause.TabIndex = 15;
            this.btnPlayPause.Text = "Play/Pause";
            this.btnPlayPause.UseVisualStyleBackColor = true;
            this.btnPlayPause.Click += new System.EventHandler(this.btnPlayPause_Click);
            // 
            // btnFullscreen
            // 
            this.btnFullscreen.Location = new System.Drawing.Point(425, 528);
            this.btnFullscreen.Margin = new System.Windows.Forms.Padding(5, 4, 5, 4);
            this.btnFullscreen.Name = "btnFullscreen";
            this.btnFullscreen.Size = new System.Drawing.Size(125, 32);
            this.btnFullscreen.TabIndex = 14;
            this.btnFullscreen.Text = "Fullscreen";
            this.btnFullscreen.UseVisualStyleBackColor = true;
            this.btnFullscreen.Click += new System.EventHandler(this.btnFullscreen_Click);
            // 
            // lblVideo
            // 
            this.lblVideo.Font = new System.Drawing.Font("Verdana", 26.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(0)));
            this.lblVideo.Location = new System.Drawing.Point(20, 577);
            this.lblVideo.Margin = new System.Windows.Forms.Padding(5, 0, 5, 0);
            this.lblVideo.Name = "lblVideo";
            this.lblVideo.Size = new System.Drawing.Size(1130, 90);
            this.lblVideo.TabIndex = 13;
            this.lblVideo.Text = "Video Name";
            this.lblVideo.TextAlign = System.Drawing.ContentAlignment.MiddleCenter;
            // 
            // lstVideos
            // 
            this.lstVideos.FormattingEnabled = true;
            this.lstVideos.ItemHeight = 18;
            this.lstVideos.Location = new System.Drawing.Point(1160, 14);
            this.lstVideos.Margin = new System.Windows.Forms.Padding(5, 4, 5, 4);
            this.lstVideos.Name = "lstVideos";
            this.lstVideos.Size = new System.Drawing.Size(336, 652);
            this.lstVideos.TabIndex = 12;
            this.lstVideos.SelectedIndexChanged += new System.EventHandler(this.lstVideos_SelectedIndexChanged);
            // 
            // pnlVideo
            // 
            this.pnlVideo.Location = new System.Drawing.Point(20, 14);
            this.pnlVideo.Margin = new System.Windows.Forms.Padding(5, 4, 5, 4);
            this.pnlVideo.Name = "pnlVideo";
            this.pnlVideo.Size = new System.Drawing.Size(1130, 492);
            this.pnlVideo.TabIndex = 11;
            // 
            // tmrVideo
            // 
            this.tmrVideo.Tick += new System.EventHandler(this.tmrVideo_Tick);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(10F, 18F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(1518, 683);
            this.Controls.Add(this.btnVolume);
            this.Controls.Add(this.trackVolume);
            this.Controls.Add(this.btnNext);
            this.Controls.Add(this.btnPrevious);
            this.Controls.Add(this.lblVideoPosition);
            this.Controls.Add(this.btnPlayPause);
            this.Controls.Add(this.btnFullscreen);
            this.Controls.Add(this.lblVideo);
            this.Controls.Add(this.lstVideos);
            this.Controls.Add(this.pnlVideo);
            this.KeyPreview = true;
            this.Margin = new System.Windows.Forms.Padding(5, 4, 5, 4);
            this.Name = "Form1";
            this.Text = "MyVideoPlayer";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.KeyDown += new System.Windows.Forms.KeyEventHandler(this.Form1_KeyDown);
            ((System.ComponentModel.ISupportInitialize)(this.trackVolume)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Button btnVolume;
        private System.Windows.Forms.TrackBar trackVolume;
        private System.Windows.Forms.Button btnNext;
        private System.Windows.Forms.Button btnPrevious;
        private System.Windows.Forms.Label lblVideoPosition;
        private System.Windows.Forms.Button btnPlayPause;
        private System.Windows.Forms.Button btnFullscreen;
        private System.Windows.Forms.Label lblVideo;
        private System.Windows.Forms.ListBox lstVideos;
        private System.Windows.Forms.Panel pnlVideo;
        private System.Windows.Forms.Timer tmrVideo;
    }
}

