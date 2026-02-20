# Learn-OpenGL
Becoming a slave to the open graphics library.

This project is configured for **Visual Studio 2022 (x64)**. All necessary dependencies (GLFW and GLAD) are included in the repository.

### Steps to Run
1. **Clone the Repository:**
   ```bash
   git clone https://github.com/j-2k/Learn-OpenGL.git
   ```

2. **Open the Project:**  
Navigate to the folder and double-click **LearnOPGL.sln**.

3. **Set Configuration:**  
Ensure the top toolbar in Visual Studio is set to **Debug (or Release) and x64**.

4. **Build and Run:**  
Press F5 or click the Local Windows Debugger button.


## No dependencies folder exists
This section will highlight what are the required dependencies & how to get them, for this project. This is here in the case that I decide to git ignore the dependencies folder.

Required Dependencies & Folder Architecture:
* **OpenGL 3.3 (Core Profile)**

* **GLFW (Window & Input Management)** - https://www.glfw.org/download.html
  * Version 3.4 or higher
  * *Just download the 64-bit Windows binaries (pre-compiled).*

* **GLAD (Extension Loader)** - https://glad.dav1d.de/
  * Check the image below. First set the red boxes, then finally, press generate & download the glad.zip file.
<img width="500"  alt="Screenshot_1" src="https://github.com/user-attachments/assets/19c47266-5bf0-4959-a498-e2b64be4857f" />



* **Folder Architecture**
  * `Dependencies/`
    * `include/` — Header files (.h)
    * `lib/` — Static library files (.lib)
  * `LearnOPGL/`
    * `src/` — All .cpp source files
