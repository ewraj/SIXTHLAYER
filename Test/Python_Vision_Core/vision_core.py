import os
import sys

try:
    from PIL import Image
    from google import genai
except ImportError:
    print("ERROR: Missing required libraries.")
    print("Please install them using: pip install google-genai pillow")
    sys.exit(1)

def analyze_image(image_path):
    if 'GEMINI_API_KEY' not in os.environ:
        print("ERROR: GEMINI_API_KEY environment variable not set.")
        print("Set it using: export GEMINI_API_KEY='your_api_key'")
        sys.exit(1)
        
    try:
        image = Image.open(image_path)
    except Exception as e:
        print(f"ERROR: Could not open image. Make sure the file exists and is a valid image.\nDetails: {e}")
        sys.exit(1)

    print(f"[*] Establishing Neural Link to Vision Core...")
    print(f"[*] Parsing visual data from: {os.path.basename(image_path)}\n")
    
    # Initialize the client (automatically picks up GEMINI_API_KEY)
    try:
        client = genai.Client()
    except Exception as e:
        print(f"ERROR: Could not initialize Gemini client: {e}")
        sys.exit(1)

    # -------------------------------------------------------------------------
    # The "BRUTAL" Tactical Directive
    # -------------------------------------------------------------------------
    prompt = """
    You are a tactical AR vision module.
    Identify the most important thing I'm looking at. 
    Act like a human partner texting me, but KEEP IT ULTRA-CONCISE.
    
    Output MUST be under 40 characters total (5-7 words max).
    Focus on non-obvious, relevant, or dangerous objects/text.
    No formatting, no tags, no conversational filler. Just the direct phrase.
    """

    try:
        # Send the multimodal request
        response = client.models.generate_content(
            model='gemini-2.5-flash',
            contents=[image, prompt]
        )
        
        result = response.text.strip()
        print(f"\n> {result}\n")
        
    except Exception as e:
        print(f"\n[!] V-CORE SYSTEM FAILURE: {e}\n")

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: python vision_core.py <drag_and_drop_image_here>")
        sys.exit(1)
        
    image_file = sys.argv[1].strip()
    
    # Remove surrounding quotes if the terminal drag-and-drop added them
    if image_file.startswith("'") and image_file.endswith("'"):
        image_file = image_file[1:-1]
    if image_file.startswith('"') and image_file.endswith('"'):
        image_file = image_file[1:-1]
        
    analyze_image(image_file)
