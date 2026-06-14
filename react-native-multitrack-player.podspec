require "json"

package = JSON.parse(File.read(File.join(__dir__, "package.json")))

Pod::Spec.new do |s|
  s.name         = "react-native-multitrack-player"
  s.version      = package["version"]
  s.summary      = package["description"]
  s.homepage     = "https://github.com/your-org/react-native-multitrack-player"
  s.license      = package["license"]
  s.authors      = package["author"]
  s.platforms    = { :ios => "13.0" }

  s.source       = { :git => "https://github.com/your-org/react-native-multitrack-player.git", :tag => "#{s.version}" }

  s.source_files = [
    "ios/**/*.{h,mm}",
    "cpp/**/*.{h,cpp}",
    "Superpowered/OpenSource/SuperpoweredIOSAudioIO.{h,mm}"
  ]

  s.vendored_frameworks = "Superpowered/libSuperpoweredAudio.xcframework"

  s.frameworks = "AVFoundation", "AudioToolbox", "CoreAudio"

  s.pod_target_xcconfig = {
    "CLANG_CXX_LANGUAGE_STANDARD" => "c++17",
    "CLANG_CXX_LIBRARY" => "libc++",
    "HEADER_SEARCH_PATHS" => "$(PODS_TARGET_SRCROOT)/Superpowered $(PODS_TARGET_SRCROOT)/cpp"
  }

  s.dependency "React-Core"
  s.dependency "ReactCommon/turbomodule/core"
end
