# 🚀 Meteor Escape

**Meteor Escape** is a 2D survival game developed in **C++** using **OpenGL** and **GLUT**. Navigate your spaceship using keyboard controls and dodge incoming meteors for as long as you can. The game features real-time animations, collision detection, and a scoring system to challenge your reflexes.

---

## 🎮 Features

- Spaceship control with **W, A, S, D** keys  
- Randomized meteor generation and movement  
- Collision detection and game-over handling  
- Visual effects including explosions and color transitions  
- Scoring based on survival time

---

## ✅ How to Run the Game

### ✅ Option 1: Run in **Code::Blocks (Windows)**

To run the game using **Code::Blocks** on Windows:

1. Open **Code::Blocks**.
2. Go to `File → New → Project`.
3. Choose **GLUT Project** and click **Go**.
4. Click **Next**, name your project (e.g., *Meteor Escape*), and choose a folder.
5. Select your compiler (**MinGW** recommended) and click **Next**.
6. Once the project is created:
    - Delete the default `main.cpp`.
    - Add the provided `main.cpp`:
        - Right-click on **Sources → Add files…**
        - Select and add your `main.cpp` file.
7. Press **F9** or click **Build and Run**.

✅ The game will now launch in a new window.  
Use **W, A, S, D** keys to move the spaceship and avoid meteors!

---

### ✅ Option 2: Run in **VS Code (Windows)**

> Make sure **OpenGL** and **GLUT** are installed and environment is configured.

#### 🔧 Setup Instructions:

1. **Download and Install GLUT**:
   - Download GLUT from:  
     [http://www.transmissionzero.co.uk/software/freeglut-devel/](http://www.transmissionzero.co.uk/software/freeglut-devel/)
   - Extract the files and:
     - Copy `.h` files to:  
       `C:\Program Files\mingw-w64\...\include\GL\`
     - Copy `.lib` or `.a` files to:  
       `C:\Program Files\mingw-w64\...\lib\`
     - Copy `.dll` to:  
       `C:\Windows\System32`

2. **Add MinGW to PATH**:
   - Open Environment Variables → System Variables → `Path`
   - Add:  
     `C:\Program Files\mingw-w64\...\bin`

3. **Compile and Run**:
   Open the project folder in VS Code and in the terminal run:

   ```bash
   g++ main.cpp -o MeteorEscape -lGL -lGLU -lglut
   ./MeteorEscape
